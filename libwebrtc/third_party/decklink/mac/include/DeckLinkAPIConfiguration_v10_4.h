/* -LICENSE-START-
** Copyright (c) 2015 Blackmagic Design
**
** Permission is hereby granted, free of charge, to any person or organization
** obtaining a copy of the software and accompanying documentation covered by
** this license (the "Software") to use, reproduce, display, distribute,
** execute, and transmit the Software, and to prepare derivative works of the
** Software, and to permit third-parties to whom the Software is furnished to
** do so, all subject to the following:
**
** The copyright notices in the Software and this entire statement, including
** the above license grant, this restriction and the following disclaimer,
** must be included in all copies of the Software, in whole or in part, and
** all derivative works of the Software, unless such copies or derivative
** works are solely in the form of machine-executable object code generated by
** a source language processor.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
** SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
** FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
** ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
** DEALINGS IN THE SOFTWARE.
** -LICENSE-END-
*/

#ifndef BMD_DECKLINKAPICONFIGURATION_v10_4_H
#define BMD_DECKLINKAPICONFIGURATION_v10_4_H

#include "DeckLinkAPIConfiguration.h"

// Interface ID Declarations

BMD_CONST REFIID IID_IDeckLinkConfiguration_v10_4                       = /* 1E69FCF6-4203-4936-8076-2A9F4CFD50CB */ {0x1E,0x69,0xFC,0xF6,0x42,0x03,0x49,0x36,0x80,0x76,0x2A,0x9F,0x4C,0xFD,0x50,0xCB};

//
// Forward Declarations

class IDeckLinkConfiguration_v10_4;

/* Interface IDeckLinkConfiguration_v10_4 - DeckLink Configuration interface */

class IDeckLinkConfiguration_v10_4 : public IUnknown
{
public:
    virtual HRESULT SetFlag (/* in */ BMDDeckLinkConfigurationID cfgID, /* in */ bool value) = 0;
    virtual HRESULT GetFlag (/* in */ BMDDeckLinkConfigurationID cfgID, /* out */ bool *value) = 0;
    virtual HRESULT SetInt (/* in */ BMDDeckLinkConfigurationID cfgID, /* in */ int64_t value) = 0;
    virtual HRESULT GetInt (/* in */ BMDDeckLinkConfigurationID cfgID, /* out */ int64_t *value) = 0;
    virtual HRESULT SetFloat (/* in */ BMDDeckLinkConfigurationID cfgID, /* in */ double value) = 0;
    virtual HRESULT GetFloat (/* in */ BMDDeckLinkConfigurationID cfgID, /* out */ double *value) = 0;
    virtual HRESULT SetString (/* in */ BMDDeckLinkConfigurationID cfgID, /* in */ CFStringRef value) = 0;
    virtual HRESULT GetString (/* in */ BMDDeckLinkConfigurationID cfgID, /* out */ CFStringRef *value) = 0;
    virtual HRESULT WriteConfigurationToPreferences (void) = 0;

protected:
    virtual ~IDeckLinkConfiguration_v10_4 () {} // call Release method to drop reference count
};


#endif /* defined(BMD_DECKLINKAPICONFIGURATION_v10_4_H) */
