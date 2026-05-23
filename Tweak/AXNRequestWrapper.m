#import "AXNRequestWrapper.h"

@implementation AXNRequestWrapper

+(instancetype)wrapRequest:(NCNotificationRequest *)request {
    AXNRequestWrapper *wrapper = [[AXNRequestWrapper alloc] init];
    wrapper.request = request;
    return wrapper;
}

-(NSString *)notificationIdentifier {
    return [self.request notificationIdentifier];
}

@end
