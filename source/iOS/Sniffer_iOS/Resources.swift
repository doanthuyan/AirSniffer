//
//  Sniffer.swift
//  Sniffer_iOS
//
//  Created by Dang Duc Nam on 7/12/18.
//  Copyright © 2018 Axon Active VN. All rights reserved.
//
import UIKit
import CoreLocation

/// Result of loading a resource
public enum Result<T> {
    
    ///
    case success(T)
    
    ///
    case failure(Error)
}

public enum PollutantDataCode:String {
    case PM25 = "PM2.5"
    case PM10 = "PM10"
    case TEMP = "TEMP"
    case HUM = "HUM"
    func code() -> String {
        return self.rawValue
    }
}

public enum SnifferStatus:String {
    case OPERATING = "OPERATING"
    case INACTIVE = "INACTIVE"
    func code() -> String {
        return self.rawValue
    }
}

public struct SmartConfigServer {
    static let endpoint: String = "192.168.4.1"
    static let configApi: String = "/config"
    static let updateApi: String = "/wifi_config/verify"
    static let resultApi: String = "/wifi_config/result"
    static let SnifferWifiName: String = "SNIFFER"
}

public struct AirSnifferServer {
    static let authorization: String = "Basic \(Bundle.sniffer.object(forInfoDictionaryKey: "AuthorizationToken") as! String)"
    static let contentType: String = "application/json"
    static let controlCache: String = "no-cache"
    //static let endpoint: String = "airsniffer.org"
    static let endpoint: String = Bundle.sniffer.object(forInfoDictionaryKey: "Endpoint") as! String
    static let queryApi: String = Bundle.sniffer.object(forInfoDictionaryKey: "QueryAPI") as! String
//    static let buildURL: String = Bundle.sniffer.object(forInfoDictionaryKey: "BuildURL") as! String
//    static let introURL: String = Bundle.sniffer.object(forInfoDictionaryKey: "IntroURL") as! String
//    static let aboutURL: String = Bundle.sniffer.object(forInfoDictionaryKey: "AboutURL") as! String
    static let pollutantURL: String = Bundle.sniffer.object(forInfoDictionaryKey: "PollutantAPI") as! String
}

struct Constants {
    static let map_api_key: String = Bundle.sniffer.object(forInfoDictionaryKey: "GoogleMapAPIKey") as! String
    static let button_color_normal: UIColor = UIColor(red: 94.0/255.0, green: 187.0/255.0, blue: 198.0/255.0, alpha: 1.0)
    static let button_color_gray: UIColor = UIColor(red: 153.0/255.0, green: 153.0/255.0, blue: 153.0/255.0, alpha: 1.0)
}

struct GlobalVar {
    static var lastLocation: CLLocation = CLLocation(latitude: 0, longitude: 0)
    static var sniffers: [Sniffer] = []
}
