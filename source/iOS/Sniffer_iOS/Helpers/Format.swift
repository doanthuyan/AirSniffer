//
//  Format.swift
//  Sniffer_iOS
//
//  Created by Dang Duc Nam on 7/16/18.
//  Copyright © 2018 Axon Active VN. All rights reserved.
//

import Foundation
extension Double {
    func format(f: String) -> String {
        return String(format: "%\(f)f", self)
    }
}
