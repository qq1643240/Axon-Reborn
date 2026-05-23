#import "RandomHeaders.h"

@interface AXNRequestWrapper : NSObject

@property (nonatomic, weak) NCNotificationRequest *request;

+(instancetype)wrapRequest:(NCNotificationRequest *)request;
-(NSString *)notificationIdentifier;

@end
