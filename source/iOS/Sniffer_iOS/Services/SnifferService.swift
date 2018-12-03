//
//  SnifferService.swift
//  Sniffer_iOS
//
//  Created by Dang Duc Nam on 7/12/18.
//  Copyright © 2018 Axon Active VN. All rights reserved.
//

import Foundation
import Alamofire
import AlamofireObjectMapper

struct SnifferService {
    private static var configURL: URL {
        return URL(string: "http://\(SmartConfigServer.endpoint)\(SmartConfigServer.configApi)")!
    }
    private static var updateURL: URL {
        return URL(string: "http://\(SmartConfigServer.endpoint)\(SmartConfigServer.updateApi)")!
    }
    private static var resultURL: URL {
        return URL(string: "http://\(SmartConfigServer.endpoint)\(SmartConfigServer.resultApi)")!
    }
    private static var queryURL: URL {
        return URL(string: "https://\(AirSnifferServer.endpoint)\(AirSnifferServer.queryApi)")!
    }
    static func pollutantURL(snifferID: String) -> URL {
        let today = Date()
//        let sinceDate = today.addingTimeInterval(12.0 * 60.0 * 60.0)
        let dateFormatter = DateFormatter()
//        dateFormatter.dateFormat = "yyyy-MM-dd'T'HH:mm:ss.SSS"
        dateFormatter.dateFormat = "yyyy-MM-dd"
        let date = dateFormatter.string(from: today)
        /*let search:String = """
        {"criteria":[{"field":"measuredAt","expression": "gt","value":"\(date)Z"}, {"field":"sourceId", "expression":"like", "value":"\(snifferID)"}]}
        """*/
        let search:String = """
        %7B%22criteria%22%3A%5B%7B%22field%22%3A%22measuredAt%22%2C%22expression%22%3A+%22gt%22%2C%22value%22%3A%22\(date)T00%3A00%3A00.000Z%22%7D%2C+%7B%22field%22%3A%22sourceId%22%2C+%22expression%22%3A%22like%22%2C+%22value%22%3A%22\(snifferID)%22%7D%5D%7D
        """
        let pollutantURL:String  = AirSnifferServer.pollutantURL
        let searchApi = String(format: pollutantURL, search)
        return URL(string: "https://\(AirSnifferServer.endpoint)\(searchApi)")!
    }
    
    static func getAllSniffers(_ completion: @escaping (Result<[Sniffer]>) -> Void) {
        let headers = [
            "Content-Type": AirSnifferServer.contentType,
            "Authorization": AirSnifferServer.authorization,
            "Cache-Control": AirSnifferServer.controlCache
        ]
        Alamofire.request(queryURL, method: .get, headers: headers).responseObject { (response: DataResponse<SnifferResponse>) in
            guard let snifferResponse = response.result.value, response.error == nil else {
                if let httpStatus = response.response {
                    switch httpStatus.statusCode {
                    case 400:
                        completion(.failure(SnifferServiceError.serverNotFound))
                    case 500:
                        completion(.failure(SnifferServiceError.serviceError))
                    default:
                        completion(.failure(SnifferServiceError.noData))
                    }
                } else {
                    completion(.failure(SnifferServiceError.noData))
                }
                return
            }
            completion(.success(snifferResponse.data))
        }
    }
    
    static func getSnifferPollutant(snifferID:String, completion: @escaping (Result<Pollutant>) -> Void) {
        let headers = [
            "Content-Type": AirSnifferServer.contentType,
            "Authorization": AirSnifferServer.authorization,
            "Cache-Control": AirSnifferServer.controlCache
        ]
        let searchURL = pollutantURL(snifferID: snifferID)
        Alamofire.request(searchURL, method: .get, headers: headers).responseObject { (response: DataResponse<Pollutant>) in
            guard let snifferResponse = response.result.value, response.error == nil else {
                completion(.failure(SnifferServiceError.noData))
                return
            }
            completion(.success(snifferResponse))
        }
    }
    
    static func getSnifferConfig(_ completion: @escaping (Result<ConfigResponse>) -> Void) {
        Alamofire.request(configURL, method: .get).responseObject { (response: DataResponse<ConfigResponse>) in
            guard let configResponse = response.result.value, response.error == nil else {
                completion(.failure(SnifferServiceError.snifferReturnError))
                return
            }
            completion(.success(configResponse))
        }
    }
    /*
    static func getMacAddress(_ completion: @escaping (Result<String>) -> Void) {
        Alamofire.request(MacAddressURL, method: .get).responseObject { (response: DataResponse<MacAddress>) in
            guard let macAddress = response.result.value, response.error == nil else {
                completion(.failure(SnifferServiceError.noData))
                return
            }
            completion(.success(macAddress.mac_address))
        }
    }
 */
    static func sendConfigToSniffer(params: [String:String],_ completion: @escaping (Result<Bool>) -> Void) {
        Alamofire.request(updateURL, method: .post, parameters: params, encoding: URLEncoding.default).response { response in
            if response.error != nil {
                completion(.failure(SnifferServiceError.canNotSendConfigToSniffer))
                return
            }
            if let data = response.data, let utf8Text = String(data: data, encoding: .utf8) {
                if utf8Text.range(of: "Verifying Configuration") != nil {
                    completion(.success(true))
                }
            } else {
                completion(.failure(SnifferServiceError.snifferReturnError))
            }
        }
    }
    static func checkConfigOnSniffer(_ completion: @escaping (Result<Bool>) -> Void) {
        Alamofire.request(resultURL, method: .get).response { response in
            if response.error != nil {
                completion(.success(true))
                return
            }
            if let data = response.data, let utf8Text = String(data: data, encoding: .utf8) {
                if utf8Text.range(of: "Configuration stored") != nil {
                    completion(.success(true))
                } else if utf8Text.range(of: "Wrong configuration") != nil {
                    completion(.failure(SnifferServiceError.wrongConfig))
                }
            } else {
                completion(.failure(SnifferServiceError.snifferReturnError))
            }
        }
    }
}
enum SnifferServiceError: Error, CustomDebugStringConvertible, LocalizedError {
    case noData, canNotSendConfigToSniffer, snifferReturnError, serviceError, serverNotFound, wrongConfig
    
    public var debugDescription: String {
        switch self {
        case .wrongConfig:
            return NSLocalizedString("error.wrongConfig", comment: "")
        case .noData:
            return NSLocalizedString("error.noData", comment: "")
        case .canNotSendConfigToSniffer:
            return NSLocalizedString("error.canNotSendConfigToSniffer", comment: "")
        case .snifferReturnError:
            return NSLocalizedString("error.snifferReturnError", comment: "")
        case .serviceError:
            return NSLocalizedString("error.serviceError", comment: "")
        case .serverNotFound:
            return NSLocalizedString("error.serverNotFound", comment: "")
        }
    }
    
    public var errorDescription: String? {
        return debugDescription
    }
}

