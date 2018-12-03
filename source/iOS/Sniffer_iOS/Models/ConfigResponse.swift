//
//  SSIDs.swift
//  Sniffer_iOS
//
//  Created by Dang Duc Nam on 7/16/18.
//  Copyright © 2018 Axon Active VN. All rights reserved.
//

import ObjectMapper

class ConfigResponse : Mappable {
    var listSSID: [String] = []
    var mac_address: String = ""
    required init?(map: Map) {
        
    }
    
    func mapping(map: Map) {
        listSSID <- map["data"]
        mac_address <- map["mac_address"]
    }
}
