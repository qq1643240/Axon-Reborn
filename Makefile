export TARGET = iphone:clang:14.5:14.0
export ARCHS = arm64 arm64e
export THEOS_PACKAGE_SCHEME = rootless

include $(THEOS)/makefiles/common.mk

TWEAK_NAME = Axon

$(TWEAK_NAME)_FILES = Tweak/Tweak.xm Tweak/AXNView.m Tweak/AXNManager.m Tweak/AXNAppCell.m Tweak/AXNRequestWrapper.m
$(TWEAK_NAME)_FRAMEWORKS = UIKit CoreGraphics
$(TWEAK_NAME)_PRIVATE_FRAMEWORKS = MediaPlayer
$(TWEAK_NAME)_CFLAGS = -fobjc-arc -Wno-deprecated-declarations
$(TWEAK_NAME)_LDFLAGS = -lroothide

include $(THEOS_MAKE_PATH)/tweak.mk

after-install::
	install.exec "sbreload"

SUBPROJECTS += Preferences
include $(THEOS_MAKE_PATH)/aggregate.mk
