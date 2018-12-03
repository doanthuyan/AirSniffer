//
//  GPSCoordinate.swift
//  Sniffer_iOS
//
//  Created by Dang Duc Nam on 7/12/18.
//  Copyright © 2018 Axon Active VN. All rights reserved.
//

import ObjectMapper

class GPSCoordinate : Mappable {
    var latitude : Double?
    var longitude : Double?
    required init?(map: Map){
        
    }
    func mapping(map: Map) {
        latitude <- map["latitude"]
        longitude <- map["longitude"]
    }
}
