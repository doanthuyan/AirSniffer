//
//  FinishViewController.swift
//  Sniffer_iOS
//
//  Created by Dang Duc Nam on 7/16/18.
//  Copyright © 2018 Axon Active VN. All rights reserved.
//

import UIKit
import MBProgressHUD
//import Reachability

class FinishViewController: UIViewController {

    @IBOutlet weak var lblFinish:UILabel!
    @IBOutlet weak var lblRetry:UILabel!
    @IBOutlet weak var btnDone:UIBarButtonItem!
    @IBOutlet weak var imgDone:UIImageView!
    
    var timer : Timer?
//    let reachability = Reachability()!
    
    override func viewDidLoad() {
        super.viewDidLoad()

        // Do any additional setup after loading the view.
        self.title = NSLocalizedString("finish_view_controller.title", bundle: Bundle.sniffer, comment: "Sniffer configuration finish")
    }
    
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        imgDone.isHidden = true
        lblFinish.isHidden = true
        lblRetry.isHidden = true
    }
    
    override func viewDidAppear(_ animated: Bool) {

        super.viewDidAppear(animated)
        
        MBProgressHUD.showAdded(to: self.view, animated: true)
        
        print("View will appear")
        
        lblFinish.text = NSLocalizedString("label_finish", bundle: Bundle.sniffer, comment: "Sniffer configuration finish label")
        lblRetry.text = NSLocalizedString("label_retry", bundle: Bundle.sniffer, comment: "Sniffer configuration retry label")
        
        navigationItem.rightBarButtonItem = UIBarButtonItem(title: NSLocalizedString("general.Done", bundle: Bundle.sniffer, comment: ""), style: .done, target: self, action: #selector(done))
        navigationItem.rightBarButtonItem?.isEnabled = false
        imgDone.isHidden = true
        lblFinish.isHidden = true
        lblRetry.isHidden = true
        
//        NotificationCenter.default.addObserver(self, selector: #selector(reachabilityChanged(note:)), name: .reachabilityChanged, object: reachability)
//        do{
//            try reachability.startNotifier()
//        }catch{
//            print("could not start reachability notifier")
//        }
        startTimer()
    }
    
    func startTimer() {
        guard timer == nil else { return }
        timer = Timer.scheduledTimer(timeInterval: 15, target: self, selector: #selector(updateConfigStatus), userInfo: nil, repeats: false)
    }
    
    func stopTimer() {
        guard timer != nil else { return }
        timer?.invalidate()
        timer = nil
        print("Stop Timer")
    }

    
    override func viewWillDisappear(_ animated: Bool) {
        super.viewWillDisappear(animated)
        stopTimer()
//        reachability.stopNotifier()
//        NotificationCenter.default.removeObserver(self, name: .reachabilityChanged, object: reachability)
    }
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
//    @objc func reachabilityChanged(note: Notification) {
//        print("Reachability changed")
//        stopTimer()
//        showFinish()
//        let reachability = note.object as! Reachability
//
//        switch reachability.connection {
//        case .wifi:
//            print("Reachable via WiFi")
//        case .cellular:
//            print("Reachable via Cellular")
//        case .none:
//            print("Network not reachable")
//        }
//    }
    @objc func updateConfigStatus() {
        print("Timer finish")
        SnifferService.checkConfigOnSniffer() { [weak self] result in
            guard let `self` = self else { return }
            MBProgressHUD.hide(for: self.view, animated: false)
            
            switch result {
            case .success(let result):
                if result {
                    self.showFinish()
                }
            case .failure(let error):
                self.lblRetry.isHidden = false
                self.showAlertInfo(with: "", and: error.localizedDescription)
            }
            
        }
    }
    
    func showFinish() -> Void {
        lblFinish.isHidden = false
        imgDone.isHidden = false
        navigationItem.rightBarButtonItem?.isEnabled = true
    }
    
    /*
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        // Get the new view controller using segue.destinationViewController.
        // Pass the selected object to the new view controller.
    }
    */
    @IBAction func done(sender: UIButton!) {
        self.navigationController?.popToRootViewController(animated: false)
    }
}
private extension FinishViewController {
    
    func showAlertInfo(with title: String, and message: String?) {
        if self.viewIfLoaded?.window != nil {
            let alert = UIAlertController(title: title, message: message, preferredStyle: .alert)
            alert.addAction(UIAlertAction(title: NSLocalizedString("general.ok", bundle: Bundle.sniffer, comment: "General alert ok button"), style: .default, handler: nil))
            present(alert, animated: true, completion: nil)
        }
    }
    
}
