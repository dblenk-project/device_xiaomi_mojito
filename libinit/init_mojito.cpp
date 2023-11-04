/*
   Copyright (c) 2015, The Linux Foundation. All rights reserved.
             (C) 2016 The CyanogenMod Project.
             (C) 2019-2020 The LineageOS Project.
             (C) 2022 Paranoid Android.
             (C) 2022 The PixelExperience Project.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.
   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <vector>

#include <android-base/logging.h>
#include <android-base/properties.h>
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>
#include <sys/sysinfo.h>

using android::base::GetProperty;

void property_override(char const prop[], char const value[], bool add = true) {
    prop_info* pi;

    pi = (prop_info*)__system_property_find(prop);
    if (pi)
        __system_property_update(pi, value, strlen(value));
    else if (add)
        __system_property_add(prop, strlen(prop), value, strlen(value));
}

void load_dalvikvm_props() {
    struct sysinfo sys;
    sysinfo(&sys);

    if (sys.totalram > 5072ull * 1024 * 1024) {
        // from - phone-xhdpi-6144-dalvik-heap.mk
        property_override("dalvik.vm.heapstartsize", "16m");
        property_override("dalvik.vm.heapgrowthlimit", "256m");
        property_override("dalvik.vm.heaptargetutilization", "0.5");
        property_override("dalvik.vm.heapmaxfree", "32m");
    } else if (sys.totalram > 3072ull * 1024 * 1024) {
        // from - phone-xhdpi-4096-dalvik-heap.mk
        property_override("dalvik.vm.heapstartsize", "8m");
        property_override("dalvik.vm.heapgrowthlimit", "192m");
        property_override("dalvik.vm.heaptargetutilization", "0.6");
        property_override("dalvik.vm.heapmaxfree", "16m");
    }

    property_override("dalvik.vm.heapsize", "512m");
    property_override("dalvik.vm.heapminfree", "8m");
}

void load_vendor_props() {
    std::string hwname = GetProperty("ro.boot.hwname", "");
    if (hwname.find("sunny") != std::string::npos) {
        property_override("ro.product.device", "sunny");
        property_override("ro.product.name", "sunny");
        property_override("ro.build.fingerprint", "Redmi/sunny_global/sunny:12/RKQ1.210614.002/V14.0.8.0.SKGMIXM:user/release-keys");
        property_override("ro.vendor.build.fingerprint", "Redmi/sunny_global/sunny:12/RKQ1.210614.002/V14.0.8.0.SKGMIXM:user/release-keys");
        property_override("ro.bootimage.build.fingerprint", "Redmi/sunny_global/sunny:12/RKQ1.210614.002/V14.0.8.0.SKGMIXM:user/release-keys");
        property_override("ro.build.description", "sunny_global-user 12 SKQ1.210908.001 V14.0.8.0.SKGMIXM release-keys");
    } else {
        property_override("ro.product.device", "mojito");
        property_override("ro.product.name", "mojito");
        property_override("ro.build.fingerprint", "Redmi/mojito/mojito:12/RKQ1.210614.002/V14.0.8.0.SKGMIXM:user/release-keys");
        property_override("ro.vendor.build.fingerprint", "Redmi/mojito/mojito:12/RKQ1.210614.002/V14.0.8.0.SKGMIXM:user/release-keys");
        property_override("ro.bootimage.build.fingerprint", "Redmi/mojito/mojito:12/RKQ1.210614.002/V14.0.8.0.SKGMIXM:user/release-keys");
        property_override("ro.build.description", "mojito-user 12 SKQ1.210908.001 V14.0.8.0.SKGMIXM release-keys");
    }

    property_override("bluetooth.device.default_name", "Redmi Note 10");
    property_override("ro.product.brand", "Redmi");
    property_override("ro.product.manufacturer", "Xiaomi");
    property_override("ro.product.model", "Redmi Note 10");
    property_override("vendor.usb.product_string", "Redmi Note 10");
}

void vendor_load_properties() {
    if (access("/system/bin/recovery", F_OK) != 0) {
        load_vendor_props();
    }
    load_dalvikvm_props();
}

