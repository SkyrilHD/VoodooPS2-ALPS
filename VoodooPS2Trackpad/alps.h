/*
 * Copyright (c) 2002 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 *
 * The contents of this file constitute Original Code as defined in and
 * are subject to the Apple Public Source License Version 1.2 (the
 * "License").  You may not use this file except in compliance with the
 * License.  Please obtain a copy of the License at
 * http://www.apple.com/publicsource and read it before using this file.
 *
 * This Original Code and all software distributed under the License are
 * distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE OR NON-INFRINGEMENT.  Please see the
 * License for the specific language governing rights and limitations
 * under the License.
 *
 * @APPLE_LICENSE_HEADER_END@
 */

#include "VoodooPS2AlpsBase.h"
#include "../VoodooInput/VoodooInput/VoodooInputMultitouch/VoodooInputEvent.h"

#define ALPS_PROTO_V7		0x700	/* t3btl t4s */

#define MAX_TOUCHES     4

/*
 * enum V7_PACKET_ID - defines the packet type for V7
 * V7_PACKET_ID_IDLE: There's no finger and no button activity.
 * V7_PACKET_ID_TWO: There's one or two non-resting fingers on touchpad
 *  or there's button activities.
 * V7_PACKET_ID_MULTI: There are at least three non-resting fingers.
 * V7_PACKET_ID_NEW: The finger position in slot is not continues from
 *  previous packet.
 */
enum V7_PACKET_ID {
    V7_PACKET_ID_IDLE,
    V7_PACKET_ID_TWO,
    V7_PACKET_ID_MULTI,
    V7_PACKET_ID_NEW,
    V7_PACKET_ID_UNKNOWN,
};


/**
 * struct alps_model_info - touchpad ID table
 * @signature: E7 response string to match.
 * @command_mode_resp: For V3/V4 touchpads, the final byte of the EC response
 *   (aka command mode response) identifies the firmware minor version.  This
 *   can be used to distinguish different hardware models which are not
 *   uniquely identifiable through their E7 responses.
 * @proto_version: Indicates V1/V2/V3/...
 * @byte0: Helps figure out whether a position report packet matches the
 *   known format for this model.  The first byte of the report, ANDed with
 *   mask0, should match byte0.
 * @mask0: The mask used to check the first byte of the report.
 * @flags: Additional device capabilities (passthrough port, trackstick, etc.).
 *
 * Many (but not all) ALPS touchpads can be identified by looking at the
 * values returned in the "E7 report" and/or the "EC report."  This table
 * lists a number of such touchpads.
 */
struct alps_model_info {
    UInt8 signature[3];
    UInt8 command_mode_resp;
    UInt16 proto_version;
    UInt8 byte0, mask0;
    unsigned int flags;
};

/**
 * struct alps_nibble_commands - encodings for register accesses
 * @command: PS/2 command used for the nibble
 * @data: Data supplied as an argument to the PS/2 command, if applicable
 *
 * The ALPS protocol uses magic sequences to transmit binary data to the
 * touchpad, as it is generally not OK to send arbitrary bytes out the
 * PS/2 port.  Each of the sequences in this table sends one nibble of the
 * register address or (write) data.  Different versions of the ALPS protocol
 * use slightly different encodings.
 */
struct alps_nibble_commands {
    SInt32 command;
    UInt8 data;
};

struct alps_bitmap_point {
    int start_bit;
    int num_bits;
};

struct input_mt_pos {
    UInt32 x;
    UInt32 y;
};

/**
 * struct alps_fields - decoded version of the report packet
 * @x_map: Bitmap of active X positions for MT.
 * @y_map: Bitmap of active Y positions for MT.
 * @fingers: Number of fingers for MT.
 * @pressure: Pressure.
 * @st: position for ST.
 * @mt: position for MT.
 * @first_mp: Packet is the first of a multi-packet report.
 * @is_mp: Packet is part of a multi-packet report.
 * @left: Left touchpad button is active.
 * @right: Right touchpad button is active.
 * @middle: Middle touchpad button is active.
 * @ts_left: Left trackstick button is active.
 * @ts_right: Right trackstick button is active.
 * @ts_middle: Middle trackstick button is active.
 */
struct alps_fields {
    UInt32 x_map;
    UInt32 y_map;
    UInt32 fingers;
    
    int pressure;
    struct input_mt_pos st;
    struct input_mt_pos mt[MAX_TOUCHES];
    
    UInt32 first_mp:1;
    UInt32 is_mp:1;
    
    UInt32 left:1;
    UInt32 right:1;
    UInt32 middle:1;
    
    UInt32 ts_left:1;
    UInt32 ts_right:1;
    UInt32 ts_middle:1;
};

class ALPS;

/**
 * struct alps_data - private data structure for the ALPS driver
 * @nibble_commands: Command mapping used for touchpad register accesses.
 * @addr_command: Command used to tell the touchpad that a register address
 *   follows.
 * @proto_version: Indicates V1/V2/V3/...
 * @byte0: Helps figure out whether a position report packet matches the
 *   known format for this model.  The first byte of the report, ANDed with
 *   mask0, should match byte0.
 * @mask0: The mask used to check the first byte of the report.
 * @fw_ver: cached copy of firmware version (EC report)
 * @flags: Additional device capabilities (passthrough port, trackstick, etc.).
 * @x_max: Largest possible X position value.
 * @y_max: Largest possible Y position value.
 * @x_bits: Number of X bits in the MT bitmap.
 * @y_bits: Number of Y bits in the MT bitmap.
 * @prev_fin: Finger bit from previous packet.
 * @multi_packet: Multi-packet data in progress.
 * @multi_data: Saved multi-packet data.
 * @f: Decoded packet data fields.
 * @quirks: Bitmap of ALPS_QUIRK_*.
 */
struct alps_data {
    /* these are autodetected when the device is identified */
    const struct alps_nibble_commands *nibble_commands;
    SInt32 addr_command;
    UInt16 proto_version;
    UInt8 byte0, mask0;
    UInt8 fw_ver[3];
    int flags;
    SInt32 x_max;
    SInt32 y_max;
    SInt32 x_bits;
    SInt32 y_bits;
    unsigned int x_res;
    unsigned int y_res;
    
    SInt32 prev_fin;
    SInt32 multi_packet;
    int second_touch;
    UInt8 multi_data[6];
    struct alps_fields f;
    UInt8 quirks;
    bool PSMOUSE_BAD_DATA;
    
    int pktsize = 6;
};

// Pulled out of alps_data, now saved as vars on class
// makes invoking a little easier
typedef bool (ALPS::*hw_init)();
typedef bool (ALPS::*decode_fields)(struct alps_fields *f, UInt8 *p);
typedef void (ALPS::*process_packet)(UInt8 *packet);
//typedef void (ALPS::*set_abs_params)();

#define ALPS_QUIRK_TRACKSTICK_BUTTONS	1 /* trakcstick buttons in trackstick packet */

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// ALPS Class Declaration
//

typedef struct ALPSStatus {
    UInt8 bytes[3];
} ALPSStatus_t;

#define kPacketLengthSmall  3
#define kPacketLengthLarge  6
#define kPacketLengthMax    6
#define kDP_CommandNibble10 0xf2
#define BITS_PER_BYTE 8

// predeclure stuff
struct alps_data;

class EXPORT ALPS : public VoodooPS2TouchPadBase {
    typedef VoodooPS2TouchPadBase super;
    OSDeclareDefaultStructors( ALPS );
    
private:
    VoodooInputEvent inputEvent;
    
    alps_data priv;
    hw_init hw_init;
    decode_fields decode_fields;
    process_packet process_packet;
    //    set_abs_params set_abs_params;
    
public:
    virtual ALPS * probe(IOService *provider, SInt32 *score);
    
    bool init(OSDictionary * dict);
    
    void stop(IOService *provider);
    
protected:
    int _multiPacket;
    
    UInt8 _multiData[6];
    
    IOGBounds _bounds;
    
    virtual bool deviceSpecificInit();
    
    bool resetMouse();
    
    int alps_process_bitmap(struct alps_data *priv, struct alps_fields *f);
    
    unsigned char alps_get_packet_id_v7(UInt8 *byte);
    
    void alps_get_finger_coordinate_v7(struct input_mt_pos *mt, UInt8 *pkt, UInt8 pkt_id);
    
    int alps_get_mt_count(struct input_mt_pos *mt);
    
    bool alps_decode_packet_v7(struct alps_fields *f, UInt8 *p);
    
    void alps_process_trackstick_packet_v7(UInt8 *packet);
    
    void alps_process_touchpad_packet_v7(UInt8 *packet);
    
    void alps_process_packet_v7(UInt8 *packet);
    
    void dispatchEventsWithInfo(int xraw, int yraw, int z, int fingers, UInt32 buttonsraw);
    
    virtual void dispatchRelativePointerEventWithPacket(UInt8 *packet, UInt32 packetSize);
    
    void setTouchPadEnable(bool enable);
    
    PS2InterruptResult interruptOccurred(UInt8 data);
    
    void packetReady();
    
    bool alps_command_mode_send_nibble(int value);
    
    bool alps_command_mode_set_addr(int addr);
    
    int alps_command_mode_read_reg(int addr);
    
    bool alps_command_mode_write_reg(int addr, UInt8 value);
    
    bool alps_command_mode_write_reg(UInt8 value);
    
    bool alps_rpt_cmd(SInt32 init_command, SInt32 init_arg, SInt32 repeated_command, ALPSStatus_t *report);
    
    bool alps_enter_command_mode();
    
    bool alps_exit_command_mode();
    
    bool alps_passthrough_mode_v2(bool enable);
        
    bool alps_absolute_mode_v1_v2();
    
    int alps_monitor_mode_send_word(int word);
    
    int alps_monitor_mode_write_reg(int addr, int value);
    
    int alps_monitor_mode(bool enable);
    
    void alps_absolute_mode_v6();
    
    bool alps_get_status(ALPSStatus_t *status);
    
    bool alps_tap_mode(bool enable);
    
    bool alps_hw_init_v1_v2();
    
    bool alps_hw_init_v6();
    
    bool alps_passthrough_mode_v3(int regBase, bool enable);
    
    bool alps_absolute_mode_v3();
    
    IOReturn alps_probe_trackstick_v3_v7(int regBase);
    
    IOReturn alps_setup_trackstick_v3(int regBase);
    
    bool alps_hw_init_v3();
    
    bool alps_get_v3_v7_resolution(int reg_pitch);
    
    bool alps_hw_init_rushmore_v3();
    
    bool alps_absolute_mode_v4();
    
    bool alps_hw_init_v4();
    
    void alps_get_otp_values_ss4_v2(unsigned char index);
    
    void alps_set_defaults_ss4_v2(struct alps_data *priv);
    
    int alps_dolphin_get_device_area(struct alps_data *priv);
    
    bool alps_hw_init_dolphin_v1();
    
    bool alps_hw_init_v7();
    
    bool alps_hw_init_ss4_v2();
    
    void ps2_command_short(UInt8 command);
    
    void ps2_command(unsigned char value, UInt8 command);
        
    void set_protocol();
    
    bool matchTable(ALPSStatus_t *e7, ALPSStatus_t *ec);
    
    IOReturn identify();
    
    void restart();
};
