//
//  PollutantValue.swift
//  Sniffer
//
//  Created by Dang Duc Nam on 7/27/18.
//  Copyright © 2018 Axon Active VN. All rights reserved.
//

import ObjectMapper

class PollutantValue: Mappable {
    var value: Double?
    var measuredAt: Date?
    required init?(map: Map) {
        
    }
    
    func mapping(map: Map) {
        value <- map["value"]
        
        let dateFormatter = DateFormatter()
        dateFormatter.dateFormat = "yyyy-MM-dd'T'HH:mm:ss.SSS'X'"
        dateFormatter.timeZone = TimeZone(identifier: "UTC")
        measuredAt <- (map["measuredAt"], DateFormatterTransform(dateFormatter: dateFormatter))
    }
}
