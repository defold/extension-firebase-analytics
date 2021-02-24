#include "platform_utils.h"

#if defined(DM_PLATFORM_IOS)
#import <Foundation/Foundation.h>

void FIR_PlatformDebugInit() {
    // add this values provide opportunity to see events in Firebase Debug View
    NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
    [defaults setBool:YES forKey:@"/google/firebase/debug_mode"];
    [defaults setBool:YES forKey:@"/google/measurement/debug_mode"];
    [defaults synchronize];
}

#endif