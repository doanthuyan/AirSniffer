//
//  Address.swift
//  Sniffer_iOS
//
//  Created by Dang Duc Nam on 7/12/18.
//  Copyright © 2018 Axon Active VN. All rights reserved.
//

import ObjectMapper

class Address: Mappable {
    var id: Int?
    var objectId: Int?
    var objectKind: String?
    var country: String?
    var province: String?
    var district: String?
    var ward: String?
    var street: String?
    var houseNr: String?
    var gpsLocation: GPSCoordinate?
    var validFrom: Date?
    var validTo: Date?
    required init?(map: Map) {
        
    }
    func mapping(map: Map) {
        id <- map["id"]
        objectId <- map["objectId"]
        objectKind <- map["objectKind"]
        country <- map["country"]
        province <- map["province"]
        district <- map["district"]
        ward <- map["ward"]
        street <- map["street"]
        houseNr <- map["houseNr"]
        gpsLocation <- map["gpsLocation"]
        
        let dateFormatter = DateFormatter()
        dateFormatter.dateFormat = "yyyy-MM-dd"
        dateFormatter.timeZone = TimeZone(identifier: "UTC")
        validFrom <- (map["validFrom"], DateFormatterTransform(dateFormatter: dateFormatter))
        validTo <- (map["validTo"], DateFormatterTransform(dateFormatter: dateFormatter))
    }
    var address: String {
        get {
            var addressString: String = ""
            if let a = houseNr {
                addressString = "\(a)"
            }
            if let a = street {
                addressString = "\(addressString), \(a)"
            }
            if let a = ward {
                addressString = "\(addressString), \(a)"
            }
            if let a = district {
                addressString = "\(addressString), \(a)"
            }
            if let a = province {
                addressString = "\(addressString), \(a)"
            }
            if let a = country {
                addressString = "\(addressString), \(a)"
            }
            return addressString
        }
    }
}
