//
//  SnifferConfigurationViewController.swift
//  Sniffer_iOS
//
//  Created by Dang Duc Nam on 7/13/18.
//  Copyright © 2018 Axon Active VN. All rights reserved.
//

import UIKit
import MBProgressHUD

class SnifferConfigurationViewController: UIViewController, UITextFieldDelegate, UIPickerViewDelegate, UIPickerViewDataSource {
    @IBOutlet weak var lblWifi: UILabel!
    @IBOutlet weak var lblPassword: UILabel!
    @IBOutlet weak var lblCode: UILabel!
    @IBOutlet weak var lblLatitude: UILabel!
    @IBOutlet weak var lblLongitude: UILabel!
    @IBOutlet weak var lblMacAddress: UILabel!
    
    @IBOutlet weak var txtWifi: UITextField!
    @IBOutlet weak var txtPassword: UITextField!
    @IBOutlet weak var txtCode: UITextField!
    @IBOutlet weak var txtLatitude: UITextField!
    @IBOutlet weak var txtLongitude: UITextField!
    @IBOutlet weak var txtMacAddress: UITextField!
    
    @IBOutlet weak var btnContinue: UIButton!
    
    var ssidPicker: UIPickerView!
    
    var ssidPickerValues:[String] = []
    
    override func viewDidLoad() {
        super.viewDidLoad()

        // Do any additional setup after loading the view.
        self.title = NSLocalizedString("sniffer_configuration_view_controller.title", bundle: Bundle.sniffer, comment: "Sniffer configuration")
        
        btnContinue.setTitle(NSLocalizedString("general.Continue", comment: "Continue button"), for: .normal)
        
        lblWifi.text = NSLocalizedString("label.wifi", comment: "Wifi label")
        lblPassword.text = NSLocalizedString("label.password", comment: "Password label")
        lblCode.text = NSLocalizedString("label.code", comment: "Code label")
        lblLatitude.text = NSLocalizedString("label.latitude", comment: "Latitude label")
        lblLongitude.text = NSLocalizedString("label.longitude", comment: "Longitude label")
        lblMacAddress.text = NSLocalizedString("label.mac_address", comment: "MAC Address label")
        
        NotificationCenter.default.addObserver(self, selector: #selector(self.loadConnectedWifiSSID(notification: )), name: .UIApplicationWillEnterForeground, object: nil)
        
        ssidPicker = UIPickerView()
        
        ssidPicker.dataSource = self
        ssidPicker.delegate = self
        
        let toolBar = UIToolbar()
        toolBar.barStyle = UIBarStyle.default
//        toolBar.isTranslucent = true
//        toolBar.tintColor = Constants.button_color_gray
        toolBar.sizeToFit()
        
        let doneButton = UIBarButtonItem(title: "Done", style: UIBarButtonItemStyle.plain, target: self, action: #selector(donePicker))
        let spaceButton = UIBarButtonItem(barButtonSystemItem: UIBarButtonSystemItem.flexibleSpace, target: nil, action: nil)
        let cancelButton = UIBarButtonItem(title: "Cancel", style: UIBarButtonItemStyle.plain, target: self, action: #selector(donePicker))
        
        toolBar.setItems([cancelButton, spaceButton, doneButton], animated: false)
        toolBar.isUserInteractionEnabled = true
        
        txtWifi.inputView = ssidPicker
        txtWifi.placeholder = NSLocalizedString("label.SSID", comment: "SSID label")
        txtWifi.inputAccessoryView = toolBar
        
        MBProgressHUD.showAdded(to: self.view, animated: true)
        
        SnifferService.getSnifferConfig { [weak self] result in
            guard let `self` = self else { return }
            MBProgressHUD.hide(for: self.view, animated: false)
            
            switch result {
            case .success(let config):
                self.ssidPickerValues = config.listSSID
                self.txtMacAddress.text = config.mac_address
                for sniffer in GlobalVar.sniffers {
                    if sniffer.macAddress == config.mac_address {
                        self.txtCode.text = sniffer.code
                        if let address = sniffer.address {
                            if let location = address.gpsLocation {
                                if let lat = location.latitude {
                                    self.txtLatitude.text = "\(lat)"
                                }
                                if let lon = location.longitude {
                                    self.txtLongitude.text = "\(lon)"
                                }
                            }
                        }
                    }
                }
                print("Done scan networks")
            case .failure(let error):
                self.showAlertInfo(with: "", and: error.localizedDescription)
            }
        }
        /*
        SnifferService.getMacAddress { [weak self] result in
            guard let `self` = self else { return }
            self.spinner(start: false)
            
            switch result {
            case .success(let mac_address):
                print("Done get MAC address")
                self.txtMacAddress.text = mac_address
            case .failure(let error):
                self.showAlertInfo(with: "", and: error.localizedDescription)
            }
        }
        */
        let tapGesture = UITapGestureRecognizer(target: self, action: #selector(self.dismissKeyboard (_:)))
        self.view.addGestureRecognizer(tapGesture)

        txtLatitude.text = "\(GlobalVar.lastLocation.coordinate.latitude.format(f: ".5"))"
        txtLongitude.text = "\(GlobalVar.lastLocation.coordinate.longitude.format(f: ".5"))"
    }
    
    @objc func dismissKeyboard (_ sender: UITapGestureRecognizer) {
        self.view.endEditing(true)
    }
    
    @objc func donePicker (sender:UIBarButtonItem)
    {
        // Put something here
        self.view.endEditing(true)
    }
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    func updateConnectedSSID() {
        let SSID = getWiFiSsid()
        if SSID == "SNIFFER" {
            btnContinue.isEnabled = true
            btnContinue.isUserInteractionEnabled = true
            btnContinue.backgroundColor = Constants.button_color_normal
        } else {
            btnContinue.isEnabled = false
            btnContinue.isUserInteractionEnabled = false
            btnContinue.backgroundColor = Constants.button_color_gray
        }
    }
    /*
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        // Get the new view controller using segue.destinationViewController.
        // Pass the selected object to the new view controller.
    }
    */
    @objc func loadConnectedWifiSSID(notification: Notification){
        self.updateConnectedSSID()
    }
    deinit {
        NotificationCenter.default.removeObserver(self, name: .UIApplicationWillEnterForeground, object: nil)
    }
    // MARK: - TextField Delegate
    func textField(_ textField: UITextField,
                   shouldChangeCharactersIn range: NSRange,
                   replacementString string: String) -> Bool {
        return false
    }
    
    // MARK: - UIPickerView Delegate & DataSource
    func numberOfComponentsInPickerView(pickerView: UIPickerView) -> Int{
        return 1
    }
    func numberOfComponents(in pickerView: UIPickerView) -> Int {
        return 1
    }
    
    func pickerView(_ pickerView: UIPickerView,
                    numberOfRowsInComponent component: Int) -> Int {
        return ssidPickerValues.count
    }
    
    func pickerView(_ pickerView: UIPickerView,
                    titleForRow row: Int,
                    forComponent component: Int) -> String? {
        return ssidPickerValues[row]
    }
    func pickerView(_ pickerView: UIPickerView,
                    didSelectRow row: Int,
                    inComponent component: Int) {
        txtWifi.text = ssidPickerValues[row]
    }
}

private extension SnifferConfigurationViewController {
    @IBAction func sendConfigToSniffer(sender: UIButton!) {
        
        var params: [String:String] = [:]
        if let ssid = txtWifi.text {
            params["ssid"] = "\(ssid)"
        }
        if let password = txtPassword.text {
            params["pwd"] = "\(password)"
        }
        if let code = txtCode.text {
            params["code"] = "\(code)"
        }
        if let latitude = txtLatitude.text {
            params["lat"] = "\(latitude)"
        }
        if let longitude = txtLongitude.text {
            params["long"] = "\(longitude)"
        }
        if let mac = txtMacAddress.text {
            params["mac"] = "\(mac)"
        }
        
        MBProgressHUD.showAdded(to: self.view, animated: true)
        
        SnifferService.sendConfigToSniffer(params: params) { [weak self] result in
            guard let `self` = self else { return }
            MBProgressHUD.hide(for: self.view, animated: false)
            
            switch result {
            case .success(let result):
                if result {
                    MBProgressHUD.hide(for: self.view, animated: false)
                    self.performSegue(withIdentifier: "showConfigResult", sender: self)
                }
            case .failure(let error):
                self.showAlertInfo(with: "", and: error.localizedDescription)
            }
            
        }
    }
    func showAlertInfo(with title: String, and message: String?) {
        let alert = UIAlertController(title: title, message: message, preferredStyle: .alert)
        alert.addAction(UIAlertAction(title: NSLocalizedString("general.ok", bundle: Bundle.sniffer, comment: "General alert ok button"), style: .default, handler: nil))
        present(alert, animated: true, completion: nil)
    }
}
