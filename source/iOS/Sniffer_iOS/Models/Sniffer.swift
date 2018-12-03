//
//  Sniffer.swift
//  Sniffer_iOS
//
//  Created by Dang Duc Nam on 7/12/18.
//  Copyright © 2018 Axon Active VN. All rights reserved.
//

import ObjectMapper

class Sniffer: Mappable {
    var id: Int?
    var code: String?
    var name: String?
    var address: Address?
    var macAddress: String?
    var status: String?
    var pollutant: Pollutant?
    
    required init?(map: Map) {
        
    }
    
    func mapping(map: Map) {
        id <- map["id"]
        code <- map["code"]
        name <- map["name"]
        address <- map["address"]
        macAddress <- map["macAddress"]
        status <- map["status"]
    }
}
