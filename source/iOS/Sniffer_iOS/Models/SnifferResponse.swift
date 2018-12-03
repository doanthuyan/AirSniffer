//
//  SnifferResponse.swift
//  Sniffer_iOS
//
//  Created by Dang Duc Nam on 7/12/18.
//  Copyright © 2018 Axon Active VN. All rights reserved.
//

import ObjectMapper

class SnifferResponse: Mappable {
    var data: [Sniffer] = []
    required init?(map: Map) {
        
    }
    
    func mapping(map: Map) {
        data <- map["data"]
    }
}
