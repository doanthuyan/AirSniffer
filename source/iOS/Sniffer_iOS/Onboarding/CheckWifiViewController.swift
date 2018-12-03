//
//  CheckWifiViewController.swift
//  Sniffer_iOS
//
//  Created by Dang Duc Nam on 7/13/18.
//  Copyright © 2018 Axon Active VN. All rights reserved.
//

import UIKit

class CheckWifiViewController: UIViewController {
    @IBOutlet weak var lblConnectedSSID: UILabel!
    @IBOutlet weak var lblReturn: UILabel!
    @IBOutlet weak var ssid: UILabel!
    @IBOutlet weak var lblConnectSniffer: UILabel!
    @IBOutlet weak var btnContinue: UIButton!
    override func viewDidLoad() {
        super.viewDidLoad()

        // Do any additional setup after loading the view.
        self.title = NSLocalizedString("checkwifi_view_controller.title", bundle: Bundle.sniffer, comment: "Check Wifi view controller title")
        
        btnContinue.setTitle(NSLocalizedString("general.Continue", comment: "Continue button"), for: .normal)
        
        let device = getDeviceName()
        let htmlString = String(format: NSLocalizedString("connect_sniffer_wifi%@", comment: "Guide for connect SNIFFER wifi"), "\(device)")
        let data = htmlString.data(using: String.Encoding.unicode)!
        let attrStr = try? NSAttributedString( 
            data: data,
            options: [NSAttributedString.DocumentReadingOptionKey.documentType: NSAttributedString.DocumentType.html],
            documentAttributes: nil)
        lblConnectSniffer.attributedText = attrStr
        
        let htmlReturnString = NSLocalizedString("return_app", comment: "Return to app")
        let dataReturn = htmlReturnString.data(using: String.Encoding.unicode)!
        let attrReturnStr = try? NSAttributedString(
            data: dataReturn,
            options: [NSAttributedString.DocumentReadingOptionKey.documentType: NSAttributedString.DocumentType.html],
            documentAttributes: nil)
        lblReturn.attributedText = attrReturnStr
        
        NotificationCenter.default.addObserver(self, selector: #selector(self.loadWifiSSID(notification: )), name: .UIApplicationWillEnterForeground, object: nil)
    }
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        self.updateSSID()
    }
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    func updateSSID() {
        let SSID = getWiFiSsid()
        ssid.text = SSID
        lblConnectedSSID.text = NSLocalizedString("current_ssid", comment: "Connected SSID")
        if SSID == SmartConfigServer.SnifferWifiName {
            btnContinue.isEnabled = true
            btnContinue.isUserInteractionEnabled = true
            btnContinue.backgroundColor = Constants.button_color_normal
        } else {
            btnContinue.isEnabled = false
            btnContinue.isUserInteractionEnabled = false
            btnContinue.backgroundColor = Constants.button_color_gray
        }
    }
    @objc func loadWifiSSID(notification: Notification){
        self.updateSSID()
    }
    deinit {
        NotificationCenter.default.removeObserver(self, name: .UIApplicationWillEnterForeground, object: nil)
    }
    /*
    // MARK: - Navigation
    
    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        // Get the new view controller using segue.destinationViewController.
        // Pass the selected object to the new view controller.
        if segue.identifier == "SnifferConfiguration", let dest: SnifferConfigurationViewController = segue.destination as? SnifferConfigurationViewController {
            dest.ssid = self.ssid.text!
        }
    }
    */

}

