//
//  Pollutant.swift
//  Sniffer_iOS
//
//  Created by Dang Duc Nam on 7/30/18.
//  Copyright © 2018 Axon Active VN. All rights reserved.
//

import ObjectMapper

class Pollutant: Mappable {
    required init?(map: Map) {
        
    }
    
    func mapping(map: Map) {
        data <- map["data"]
    }
    
    var data:[PollutantData] = []
}
