#import "RandomHeaders.h"

@interface AXNView : UIView

@property (nonatomic, retain) NSMutableArray *list;
@property (nonatomic, retain) UICollectionView *collectionView;
@property (nonatomic, retain) UICollectionViewFlowLayout *collectionViewLayout;
@property (nonatomic, retain) NSString *selectedBundleIdentifier;

@property (nonatomic, assign) BOOL hapticFeedback;
@property (nonatomic, assign) BOOL badgesEnabled;
@property (nonatomic, assign) BOOL badgesShowBackground;
@property (nonatomic, assign) NSInteger darkMode;
@property (nonatomic, assign) BOOL showingLatestRequest;
@property (nonatomic, assign) NSInteger selectionStyle;
@property (nonatomic, assign) BOOL addBlur;
@property (nonatomic, assign) NSInteger style;
@property (nonatomic, assign) NSInteger sortingMode;
@property (nonatomic, assign) NSInteger showByDefault;
@property (nonatomic, assign) NSInteger alignment;
@property (nonatomic, assign) NSInteger iconStyle;
@property (nonatomic, assign) CGFloat spacing;

// 新增：手势清除开关
@property (nonatomic, assign) BOOL swipeClearEnabled;
@property (nonatomic, assign) BOOL swipeClearAllEnabled;

-(void)refresh;
-(void)reset;

/* Compatibility stuff. */
-(void)setContentHost:(id)arg1 ;
-(void)setSizeToMimic:(CGSize)arg1 ;
-(void)_layoutContentHost;
-(CGSize)sizeToMimic;
-(id)contentHost;
-(void)_updateSizeToMimic;
-(unsigned long long)_optionsForMainOverlay;

@end
