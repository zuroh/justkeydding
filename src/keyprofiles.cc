/*
MIT License

Copyright (c) 2018 Nestor Napoles

Common key-profiles for key detection algorithms

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR sOTHER DEALINGS IN THE
SOFTWARE.
*/

#include<map>
#include<vector>
#include<string>
#include<array>

#include "./keyprofiles.h"

namespace justkeydding {

KeyProfile::KeyProfile() :
    KeyProfile("temperley", "sapp") {}

KeyProfile::KeyProfile(std::string keyProfile) :
    KeyProfile(keyProfile, keyProfile) {}

KeyProfile::KeyProfile(std::string majKeyProfile, std::string minKeyProfile) {
    // Initializes the stored key profiles
    initKeyProfileArrays();
    // Can we find the ones sent to the constructor?
    if (isValidMajorKeyProfile(majKeyProfile) &&
        isValidMinorKeyProfile(minKeyProfile)) {
        m_majorKeyProfile = majKeyProfile;
        m_minorKeyProfile = minKeyProfile;
    }
    return;
}

void KeyProfile::initKeyProfileArrays() {
    // Major Keys
    // m_majorKeyProfilesOriginal["krumhansl_kessler"] = {
    //     6.35, 2.23, 3.48, 2.33, 4.38, 4.09,
    //     2.52, 5.19, 2.39, 3.66, 2.29, 2.88
    // };
    m_majorKeyProfiles["krumhansl_kessler"] = {
        0.15195022732711172, 0.0533620483369227, 0.08327351040918879,
        0.05575496530270399, 0.10480976310122037, 0.09787030390045463,
        0.06030150753768843, 0.1241923905240488, 0.05719071548217276,
        0.08758076094759511, 0.05479779851639147, 0.06891600861450106
    };
    // m_majorKeyProfileOriginal["aarden_essen"] = {
    //     17.7661, 0.145624, 14.9265, 0.160186, 19.8049, 11.3587,
    //     0.291248, 22.062, 0.145624, 8.15494, 0.232998, 4.95122
    // };
    m_majorKeyProfiles["aarden_essen"] = {
        0.17766092893562843, 0.001456239417504233, 0.1492649402940239,
        0.0016018593592562562, 0.19804892078043168, 0.11358695456521818,
        0.002912478835008466, 0.2206199117520353, 0.001456239417504233,
        0.08154936738025305, 0.002329979068008373, 0.049512180195127924
    };
    // m_majorKeyProfileOriginal["sapp"] = {
    //     2.0, 0.0, 1.0, 0.0, 1.0, 1.0,
    //     0.0, 2.0, 0.0, 1.0, 0.0, 1.0
    // };
    m_majorKeyProfiles["sapp"] = {
        0.2222222222222222, 0.0, 0.1111111111111111, 0.0,
        0.1111111111111111, 0.1111111111111111, 0.0, 0.2222222222222222,
        0.0, 0.1111111111111111, 0.0, 0.1111111111111111
    };
    // m_majorKeyProfileOriginal["bellman_budge"] = {
    //     16.8, 0.86, 12.95, 1.41, 13.49, 11.93,
    //     1.25, 20.28, 1.8, 8.04, 0.62, 10.57
    // };
    m_majorKeyProfiles["bellman_budge"] = {
        0.168, 0.0086, 0.1295, 0.0141, 0.1349, 0.1193,
        0.0125, 0.2028, 0.018000000000000002, 0.0804, 0.0062, 0.1057
    };
    // m_majorKeyProfileOriginal["temperley"] = {
    //     0.748, 0.06, 0.488, 0.082, 0.67, 0.46,
    //     0.096, 0.715, 0.104, 0.366, 0.057, 0.4,
    // };
    m_majorKeyProfiles["temperley"] = {
        0.17616580310880825, 0.014130946773433817, 0.11493170042392838,
        0.019312293923692884, 0.15779557230334432, 0.10833725859632594,
        0.02260951483749411, 0.16839378238341965, 0.02449364107395195,
        0.08619877531794629, 0.013424399434762127, 0.09420631182289213
    };
    // Minor Keys
    // m_minorKeyProfilesOriginal["krumhansl_kessler"] = {
    //     6.33, 2.68, 3.52, 5.38, 2.6, 3.53,
    //     2.54, 4.75, 3.98, 2.69, 3.34, 3.17
    // };
    m_minorKeyProfiles["krumhansl_kessler"] = {
        0.14221523253201526, 0.06021118849696697, 0.07908335205571781,
        0.12087171422152324, 0.05841383958660975, 0.07930802066951245,
        0.05706582790384183, 0.1067175915524601, 0.08941810829027184,
        0.06043585711076162, 0.07503931700741405, 0.07121995057290496
    };
    // m_minorKeyProfilesOriginal["aarden_essen"] = {
    //     18.2648, 0.737619, 14.0499, 16.8599, 0.702494, 14.4362,
    //     0.702494, 18.6161, 4.56621, 1.93186, 7.37619, 1.75623
    // };
    m_minorKeyProfiles["aarden_essen"] = {
        0.18264800547944018, 0.007376190221285707, 0.14049900421497014,
        0.16859900505797015, 0.0070249402107482066, 0.14436200433086013,
        0.0070249402107482066, 0.18616100558483017, 0.04566210136986304,
        0.019318600579558018, 0.07376190221285707, 0.017562300526869017
    };
    // m_minorKeyProfilesOriginal["sapp"] = {
    //     2.0, 0.0, 1.0, 1.0, 0.0, 1.0,
    //     0.0, 2.0, 1.0, 0.0, 0.5, 0.5
    // };
    m_minorKeyProfiles["sapp"] = {
        0.2222222222222222, 0.0, 0.1111111111111111, 0.1111111111111111,
        0.0, 0.1111111111111111, 0.0, 0.2222222222222222,
        0.1111111111111111, 0.0, 0.05555555555555555, 0.05555555555555555
    };
    // m_minorKeyProfilesOriginal["bellman_budge"] = {
    //     18.16, 0.69, 12.99, 13.34, 1.07, 11.15,
    //     1.38, 21.07, 7.49, 1.53, 0.92, 10.21
    // };
    m_minorKeyProfiles["bellman_budge"] = {
        0.1816, 0.0069, 0.12990000000000002,
        0.1334, 0.010700000000000001, 0.1115,
        0.0138, 0.2107, 0.07490000000000001,
        0.015300000000000001, 0.0092, 0.10210000000000001
    };
    // m_minorKeyProfilesOriginal["temperley"] = {
    //     0.712, 0.084, 0.474, 0.618, 0.049, 0.46,
    //     0.105, 0.747, 0.404, 0.067, 0.133, 0.33
    // };
    m_minorKeyProfiles["temperley"] = {
        0.1702127659574468, 0.020081281377002155, 0.1133158020559407,
        0.14774085584508725, 0.011714080803251255, 0.10996892182644036,
        0.02510160172125269, 0.1785799665311977, 0.09658140090843893,
        0.016017212526894576, 0.03179536218025341, 0.07889074826679417
    };
    return;
}

bool KeyProfile::isValidMajorKeyProfile(std::string keyProfile) {
    return m_majorKeyProfiles.find(keyProfile) != m_majorKeyProfiles.end();
}

bool KeyProfile::isValidMinorKeyProfile(std::string keyProfile) {
    return m_minorKeyProfiles.find(keyProfile) != m_minorKeyProfiles.end();
}

std::string KeyProfile::whichMajorKeyProfile() {
    return m_majorKeyProfile;
}
std::string KeyProfile::whichMinorKeyProfile() {
    return m_minorKeyProfile;
}
KeyProfile::KeyProfileArray KeyProfile::getMajorKeyProfile() {
    return m_majorKeyProfiles[m_majorKeyProfile];
}
KeyProfile::KeyProfileArray KeyProfile::getMinorKeyProfile() {
    return m_minorKeyProfiles[m_minorKeyProfile];
}

}  // namespace justkeydding
