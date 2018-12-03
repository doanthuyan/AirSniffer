//
//  SnifferMarkerWindowView.swift
//  Sniffer_iOS
//
//  Created by Dang Duc Nam on 7/30/18.
//  Copyright © 2018 Axon Active VN. All rights reserved.
//

import UIKit

class SnifferMarkerWindowView: UIView {
    @IBOutlet weak var lblName:UILabel!
    @IBOutlet weak var lblAddress:UILabel!
    @IBOutlet weak var lblPM25:UILabel!
    @IBOutlet weak var lblPM10:UILabel!
    @IBOutlet weak var lblHum:UILabel!
    @IBOutlet weak var lblTemp:UILabel!
    /*
    // Only override draw() if you perform custom drawing.
    // An empty implementation adversely affects performance during animation.
    override func draw(_ rect: CGRect) {
        // Drawing code
    }
    */
    class func instanceFromNib() -> UIView {
        return UINib(nibName: "SnifferMarkerWindowView", bundle: nil).instantiate(withOwner: self, options: nil).first as! UIView
    }
}
