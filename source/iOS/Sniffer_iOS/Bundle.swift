//
//  Bundle.swift
//  Sniffer_iOS
//
//  Created by Dang Duc Nam on 7/12/18.
//  Copyright © 2018 Axon Active VN. All rights reserved.
//

import Foundation
class Bundle {
    static var sniffer: Foundation.Bundle {
        //return Foundation.Bundle(for: Bundle.self)
        return Foundation.Bundle.main
    }
}

extension Foundation.Bundle {
    var releaseVersionNumber: String? {
        return infoDictionary?["CFBundleShortVersionString"] as? String
    }
    
    var buildVersionNumber: String? {
        return infoDictionary?[kCFBundleVersionKey as String] as? String
    }
}
