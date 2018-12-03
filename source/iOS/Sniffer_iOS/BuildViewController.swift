//
//  BuildViewController.swift
//  Sniffer_iOS
//
//  Created by Dang Duc Nam on 7/11/18.
//  Copyright © 2018 Axon Active VN. All rights reserved.
//

import UIKit
import WebKit
import MBProgressHUD

class BuildViewController: UIViewController {

//    var spinner: UIActivityIndicatorView!
    var webView: WKWebView!
    @IBOutlet weak var btnOnboarding: UIButton!
    
    override func viewDidLoad() {
        super.viewDidLoad()

        // Do any additional setup after loading the view.
        self.title = NSLocalizedString("build_view_controller.title", bundle: Bundle.sniffer, comment: "Build view controller title")
        btnOnboarding.setTitle(NSLocalizedString("onboarding.button", bundle: Bundle.sniffer, comment: "Onboarding button title"), for: UIControlState.normal)
        
        webView = WKWebView(frame: CGRect(x: view.frame.origin.x + 8, y: view.frame.origin.y, width: view.frame.size.width - 16, height: btnOnboarding.frame.origin.y - 16))
        webView.uiDelegate = self
        self.view.addSubview(webView)
        
//        spinner = UIActivityIndicatorView(activityIndicatorStyle: .gray)
//        webView.addSubview(spinner)
        
        webView.navigationDelegate = self
        
        MBProgressHUD.showAdded(to: self.view, animated: true)
        let bundle = Foundation.Bundle.main
        let baseURL = bundle.url(forResource: "HTML", withExtension: nil)!
        let contentURL = baseURL.appendingPathComponent("Build.htm")
        let htmlString = try? String(contentsOf: contentURL, encoding: String.Encoding.utf8)
        self.webView.loadHTMLString(htmlString!, baseURL: baseURL)
        
//        let myURL = URL(string:AirSnifferServer.buildURL)
//        let myRequest = URLRequest(url: myURL!)
//        webView.load(myRequest)
        
//        spinner.center = webView.center
//        self.spinner(start: true)
        
    }
    
    override func viewDidAppear(_ animated: Bool) {
        webView.frame = CGRect(x: view.frame.origin.x + 8, y: view.frame.origin.y, width: view.frame.size.width - 16, height: btnOnboarding.frame.origin.y - 16)
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

extension BuildViewController: WKNavigationDelegate, WKUIDelegate {
    func webView(_ webView: WKWebView, didFinish navigation: WKNavigation!) {
//        self.spinner(start: false)
        MBProgressHUD.hide(for: self.view, animated: false)
    }
}
