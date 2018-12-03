//
//  PollutantData.swift
//  Sniffer
//
//  Created by Dang Duc Nam on 7/27/18.
//  Copyright © 2018 Axon Active VN. All rights reserved.
//

import ObjectMapper

class PollutantData: Mappable {
    var values: [PollutantValue] = []
    var code: String?
    var sensor: String?
    var defaultUnit: String?
    required init?(map: Map) {
        
    }
    
    func mapping(map: Map) {
        values <- map["values"]
        code <- map["code"]
        sensor <- map["sensor"]
        defaultUnit <- map["defaultUnit"]
    }
}
