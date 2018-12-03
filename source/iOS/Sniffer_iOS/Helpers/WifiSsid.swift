//
//  WifiSsid.swift
//  Sniffer_iOS
//
//  Created by Dang Duc Nam on 7/13/18.
//  Copyright © 2018 Axon Active VN. All rights reserved.
//

import Foundation
import SystemConfiguration.CaptiveNetwork

func getWiFiSsid() -> String? {
    var ssid: String?
    if let interfaces = CNCopySupportedInterfaces() as NSArray? {
        for interface in interfaces {
            if let interfaceInfo = CNCopyCurrentNetworkInfo(interface as! CFString) as NSDictionary? {
                ssid = interfaceInfo[kCNNetworkInfoKeySSID as String] as? String
                break
            }
        }
    }
    return ssid
}
