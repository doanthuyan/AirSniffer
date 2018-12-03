//
//  MapViewController.swift
//  Sniffer_iOS
//
//  Created by Dang Duc Nam on 7/11/18.
//  Copyright © 2018 Axon Active VN. All rights reserved.
//

import UIKit
import Alamofire
import AlamofireObjectMapper
import GoogleMaps
import MBProgressHUD

class MapViewController: UIViewController {
    
    private var infoWindow = SnifferMarkerWindowView()
    fileprivate var locationMarker : GMSMarker? = GMSMarker()

    var locationManager = CLLocationManager()

    lazy var mapView = GMSMapView()
    
    private var sniffers: [Sniffer] = [] {
        didSet {
            defer {
                loadSniffers()
            }
            guard !sniffers.isEmpty else {
                return
            }
        }
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()

        self.infoWindow = loadNiB()
        
        self.title = NSLocalizedString("map_view_controller.title", bundle: Bundle.sniffer, comment: "Map view controller title")

        let camera = GMSCameraPosition.camera(withLatitude: 10.809611, longitude: 106.664159, zoom: 13.0)
        mapView = GMSMapView.map(withFrame: CGRect.zero, camera: camera)
        mapView.isMyLocationEnabled = true
        mapView.delegate = self
        view = mapView

        // User Location
        locationManager.delegate = self
        locationManager.requestWhenInUseAuthorization()
        locationManager.desiredAccuracy = kCLLocationAccuracyBest
        locationManager.startUpdatingLocation()
        
        // Do any additional setup after loading the view.
        MBProgressHUD.showAdded(to: self.view, animated: true)

        SnifferService.getAllSniffers { [weak self] result in
            guard let `self` = self else { return }
            switch result {
            case .success(let sniffers):
                self.sniffers = sniffers
                GlobalVar.sniffers = sniffers
                MBProgressHUD.hide(for: self.view, animated: false)
            case .failure(let error):
                MBProgressHUD.hide(for: self.view, animated: false)
                self.showAlertInfo(with: "", and: error.localizedDescription)
            }
        }
        
    }
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    

    /*
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        // Get the new view controller using segue.destinationViewController.
        // Pass the selected object to the new view controller.
    }
    */
    
}
extension MapViewController: CLLocationManagerDelegate {
    func locationManager(_ manager: CLLocationManager, didUpdateLocations locations: [CLLocation]) {
        let userLocation = locations.last
        let camera = GMSCameraPosition.camera(withLatitude: userLocation!.coordinate.latitude,
                                              longitude: userLocation!.coordinate.longitude, zoom: 13.0)
        mapView.animate(to: camera)
        
        GlobalVar.lastLocation = locations.last!
        
        locationManager.stopUpdatingLocation()
    }
}

extension MapViewController: GMSMapViewDelegate {
    
    func loadNiB() -> SnifferMarkerWindowView {
        let infoWindow = SnifferMarkerWindowView.instanceFromNib() as! SnifferMarkerWindowView
        return infoWindow
    }
    
    func mapView(_ mapView: GMSMapView, didTap marker: GMSMarker) -> Bool {
        
        var markerData : MarkerData
        
        if let data = marker.userData {
            markerData = data as! MarkerData
        } else {
            return true
        }
        
        locationMarker = marker
        infoWindow.removeFromSuperview()
        infoWindow = loadNiB()
        
        guard let location = locationMarker?.position else {
            print("locationMarker is nil")
            return false
        }
        infoWindow.alpha = 0.9
        infoWindow.layer.cornerRadius = 12
        infoWindow.layer.borderWidth = 2
        infoWindow.layer.borderColor = Constants.button_color_normal.cgColor
        
        if let a = markerData.name {
            infoWindow.lblName.text = a
        }
        if let a = markerData.address {
            infoWindow.lblAddress.text = String(format: "%@: \(a)", NSLocalizedString("general.Address", comment: "Address"))
        }
        if let a = markerData.PM25 {
            infoWindow.lblPM25.text = "PM 2.5: \(a)"
        }
        if let a = markerData.PM10 {
            infoWindow.lblPM10.text = "PM 10: \(a)"
        }
        if let a = markerData.Temp {
            infoWindow.lblTemp.text = String(format: "%@: \(a)", NSLocalizedString("general.Temperature", comment: "Temperature"))
        }
        if let a = markerData.Hum {
            infoWindow.lblHum.text = String(format: "%@: \(a)", NSLocalizedString("general.Humidity", comment: "Humidity"))
        }
        
        infoWindow.center = mapView.projection.point(for: location)
        infoWindow.center.y = infoWindow.center.y - 82
        self.view.addSubview(infoWindow)
        
        return true
    }
    
    func mapView(_ mapView: GMSMapView, didChange position: GMSCameraPosition) {
        if (locationMarker != nil){
            guard let location = locationMarker?.position else {
                print("locationMarker is nil")
                return
            }
            infoWindow.center = mapView.projection.point(for: location)
            infoWindow.center.y = infoWindow.center.y - 82
        }
    }
    
    func mapView(_ mapView: GMSMapView, idleAt position: GMSCameraPosition) {
        print("MAP IDLE AT \(position.target.latitude) \(position.target.longitude)")
        loadSniffers()
    }
    
    func mapView(_ mapView: GMSMapView, didTapAt coordinate: CLLocationCoordinate2D) {
        infoWindow.removeFromSuperview()
    }
    
    func loadSniffers() -> Void {
        mapView.clear()
        for sniffer in sniffers {
            if sniffer.status == SnifferStatus.OPERATING.code() {
                let snifferCoordinate = CLLocationCoordinate2D(latitude: (sniffer.address?.gpsLocation?.latitude)!, longitude: (sniffer.address?.gpsLocation?.longitude)!)

                if !mapView.projection.contains(snifferCoordinate) {
                    continue
                }
                if let a = sniffer.name {
                    print("Adding sniffer \(a)")
                }

                if sniffer.pollutant != nil {
                    let marker = createMarker(sniffer: sniffer, coordinate: snifferCoordinate)
                    marker.map = mapView
                    continue
                }
                
                SnifferService.getSnifferPollutant(snifferID: sniffer.code!) { [weak self] result in
                    guard let `self` = self else { return }
                    switch result {
                    case .success(let pollutant):
                        sniffer.pollutant = pollutant
                        let marker = self.createMarker(sniffer: sniffer, coordinate: snifferCoordinate)
                        marker.map = self.mapView
                    case .failure(let error):
                        self.showAlertInfo(with: "", and: error.localizedDescription)
                    }
                }
            }
        }
    }
    
//    func loadAllSniffers(completion: @escaping (Bool) -> Void) {
//        var countStarting:Int = 0
//        var countFinish:Int = 0
//        for sniffer in sniffers {
//            if sniffer.status == SnifferStatus.OPERATING.code() {
//                if sniffer.pollutant != nil {
//                    continue
//                }
//                countStarting += 1
//
//                SnifferService.getSnifferPollutant(snifferID: sniffer.code!) { [weak self] result in
//                    guard let `self` = self else { return }
//                    switch result {
//                    case .success(let pollutant):
//                        print("Loaded sniffer pollutant for \(sniffer.name!)")
//                        sniffer.pollutant = pollutant
//                        countFinish += 1
//                        if countFinish==countStarting {
//                            print("Finish downloaded")
//                            completion(true)
//                        }
//                    case .failure(let error):
//                        countFinish += 1
//                        self.showAlertInfo(with: "", and: error.localizedDescription)
//                    }
//                }
//            }
//        }
//    }
    
    private func createMarker(sniffer: Sniffer, coordinate:CLLocationCoordinate2D) -> GMSMarker {
        let markerData:MarkerData = MarkerData()
        
        let marker = GMSMarker()
        marker.position = coordinate
        marker.title = sniffer.name
        markerData.name = sniffer.name
        marker.snippet = sniffer.address?.address
        markerData.address = sniffer.address?.address
        
        if sniffer.pollutant!.data.count>0 {
            var pm25Existed = false
            for pollutantData in sniffer.pollutant!.data {
                let pollutantValues = pollutantData.values
                if pollutantValues.count > 0 {
                    let pollutantValue = pollutantValues[0]
                    if let value = pollutantValue.value {
                        switch pollutantData.code {
                        case PollutantDataCode.PM25.code():
                            markerData.PM25 = value
                        case PollutantDataCode.PM10.code():
                            markerData.PM10 = value
                        case PollutantDataCode.TEMP.code():
                            markerData.Temp = value
                        case PollutantDataCode.HUM.code():
                            markerData.Hum = value
                        case .none:
                            break
                        case .some(_):
                            break
                        }
                    }
                }
                
                if pollutantData.code == PollutantDataCode.PM25.code() {
                    pm25Existed = true
                    
                    if pollutantValues.count > 0 {
                        let currentPollutantPM25:PollutantValue = pollutantValues[0]
                        
                        let healthyRange: ClosedRange<Double> = (0...12)
                        let moderateRange: ClosedRange<Double> = (12.1...35.5)
                        let unhealthyRange: ClosedRange<Double> = (35.6...300)
                        switch currentPollutantPM25.value! {
                        case healthyRange:
                            marker.icon = UIImage(named: "ico_healthy")
                        case moderateRange:
                            marker.icon = UIImage(named: "ico_moderate")
                        case unhealthyRange:
                            marker.icon = UIImage(named: "ico_unhealthy")
                        default:
                            marker.icon = UIImage(named: "ico_unspecified")
                        }
                    } else {
                        marker.icon = UIImage(named: "ico_unspecified")
                    }
                }
            }
            if !pm25Existed {
                marker.icon = UIImage(named: "ico_unspecified")
            }
        } else {
            marker.icon = UIImage(named: "ico_unspecified")
        }
        marker.userData = markerData
        
        return marker
    }
}

private extension MapViewController {
    
    func showAlertInfo(with title: String, and message: String?) {
        if self.viewIfLoaded?.window != nil {
            let alert = UIAlertController(title: title, message: message, preferredStyle: .alert)
            alert.addAction(UIAlertAction(title: NSLocalizedString("general.ok", bundle: Bundle.sniffer, comment: "General alert ok button"), style: .default, handler: nil))
            present(alert, animated: true, completion: nil)
        }
    }
    
//    @IBAction func showHeatmap(sender: UIButton!) {
//        MBProgressHUD.showAdded(to: self.view, animated: true)
//        loadAllSniffers() { [weak self] result in
//            guard self != nil else { return }
//            switch result {
//            case true:
//                MBProgressHUD.hide(for: (self?.view)!, animated: true)
//                self?.performSegue(withIdentifier: "heatmap", sender: self)
//            case false:
//                break
//            }
//        }
//    }
}
