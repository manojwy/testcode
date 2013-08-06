//
//  CMWebViewHelper.h
//  CloudMagic
//
//  Created by suhas on 28/03/13.
//  Copyright (c) 2013 webyog. All rights reserved.
//

#import <Foundation/Foundation.h>

/*
 
 This is helper class for webview to handle communications from js. Since, subclassing of UIWebView is not allowed this seperate class is created.

 To handle communications from javascript to native, JSBridge is used. JSBridge is set of javascript functions which will convert a native call in js to new request in native with predefined scheme - native://. That is caught in shouldStartLoadWithRequest: webview delegate and handled.
 
*/

@protocol CMWebViewHelperDelegate;

@interface CMWebViewHelper : NSObject <UIWebViewDelegate>

-(id)initWithWebView:(UIWebView *)webView delegate:(id <CMWebViewHelperDelegate> )delegate webViewDelegate:(id <UIWebViewDelegate>)webViewDelegate;

@property (nonatomic, strong) UIWebView * webView;

@property (nonatomic, weak) id <CMWebViewHelperDelegate> delegate;

@property (nonatomic, weak) id <UIWebViewDelegate> webViewDelegate;

@property (nonatomic, copy) NSString * documentURLString;
@property (nonatomic, copy) NSString * documentTitle;
@property (nonatomic, strong) NSURL * currentlyLoadingURL;

@end


@protocol CMWebViewHelperDelegate <NSObject>

-(void)webView:(UIWebView *)webView notificationFromJS:(NSDictionary *)notification;

@end