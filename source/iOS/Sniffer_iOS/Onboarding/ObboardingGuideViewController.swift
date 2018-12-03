//
//  ObboardingGuideViewController.swift
//  Sniffer_iOS
//
//  Created by Dang Duc Nam on 7/18/18.
//  Copyright © 2018 Axon Active VN. All rights reserved.
//

import UIKit

class ObboardingGuideViewController: UIViewController {

    @IBOutlet weak var lblPush:UILabel!
    @IBOutlet weak var lblBink:UILabel!
    @IBOutlet weak var lblContinue:UILabel!
    @IBOutlet weak var imageView:UIImageView!
    
    override func viewDidLoad() {
        super.viewDidLoad()

        // Do any additional setup after loading the view.
        
        self.title = NSLocalizedString("onboarding_guide_view_controller.title", bundle: Bundle.sniffer, comment: "Onboarding view controller title")
        
        self.lblPush.text = NSLocalizedString("onboarding.push", bundle: Bundle.sniffer, comment: "Label to inform press push button")
        self.lblBink.text = NSLocalizedString("onboarding.blink", bundle: Bundle.sniffer, comment: "Label to tell blink LED")
        self.lblContinue.text = NSLocalizedString("onboarding.continue", bundle: Bundle.sniffer, comment: "Label to tell press Continue")
        
        imageView.animationImages = [UIImage(named: "led_on"),UIImage(named: "led_off")] as? [UIImage]
        imageView.animationDuration = 0.5
        imageView.startAnimating()
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    

    /*
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        // Get the new view controller using segue.destinationViewController.
        // Pass the selected object to the new view controller.
    }
    */

}
