//
//  System.swift
//  Sniffer_iOS
//
//  Created by Dang Duc Nam on 7/13/18.
//  Copyright © 2018 Axon Active VN. All rights reserved.
//

import Foundation
import  UIKit
func getDeviceName() -> String {
    switch UIDevice.current.userInterfaceIdiom {
    case .phone:
        return "iPhone"
    case .pad:
        return "iPad"
    case .unspecified:
        return "device"
    case .tv:
        return "device"
    case .carPlay:
        return "device"
    }
}
