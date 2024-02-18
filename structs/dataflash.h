#pragma pack(push, 1)

// strings (like device name) contain string length in first byte

typedef struct {
  int16_t ot_chg;
  uint8_t  ot_chg_time;
  int16_t ot_chg_recovery;
  int16_t ot_dsg;
  uint8_t  ot_dsg_time;
  int16_t ot_dsg_recovery;
} subclass_0x02; // 02: Configuration: Safety

typedef struct {
  int16_t chg_inhibit_temp_low;
  int16_t chg_inhibit_temp_high;
  int16_t temp_hyst;
} subclass_0x20; // 32: Configuration: Charge Inhibit cfg

typedef struct {
  int16_t unknown_00;
  int16_t charging_voltage;
  int16_t delta_temp;
  int16_t suspend_low_temp;
  int16_t suspend_high_temp;
} subclass_0x22; // 34: Configuration: Charge

// TODO: recheck
typedef struct {
  int16_t taper_current;
  int16_t min_taper_capacity;
  int16_t taper_voltage;
  int8_t current_taper_window;
  int8_t tca_set_percent;
  int8_t tca_clear_percent;
  int8_t fc_set_percent;
  int8_t fc_clear_percent;
  int16_t dod_at_eoc_delta_t;
} subclass_0x24; // 36: Configuration: Charge termination

// TODO: get back to this. shit's fucked
typedef struct {
    int16_t unk_00;
    int16_t unk_02;
    int16_t unk_04;
    int16_t unk_06;
    int8_t initial_standby_current; // 0x8
    int16_t initial_max_load_current; // 0x9
    uint8_t unk_0B[5];
    uint8_t design_energy_scale; // 0x10
    uint8_t unk_11[6];

    uint16_t design_capacity; // 0x17 // 0x288
    int16_t unk_19; // 0x19
    int16_t soh_load; // 0x1B
    int8_t device_name[8]; // 0x1D
} subclass_0x30; // 48: Configuration: Data

// TODO: get back to this. shit's fucked
typedef struct {
    uint8_t soc_set_threshold; // 0x00
    uint8_t soc_clear_threshold; // 0x01
//
    uint16_t final_voltage; // 0x0F
} subclass_0x31; // 49: Configuration: Discharge

// TODO: get back to this. shit's fucked
typedef struct {
    uint8_t full_reset_counter; // 0x00 // 0x26A
    // unknown data follows. constant, but different per dataflash version
} subclass_0x38; // 56: Configuration: Integrity data

typedef struct {
    uint8_t dataflash_version; // 0x00 // 0x2AD
    // unknown data follows. constant on all dataflash versions
} subclass_0x39; // 57: System: Manufacturer info

// TODO: get back to this. shit's fucked
typedef struct {
  // constant on all dataflash versions
} subclass_0x40; // 64: Configuration: registers

// TODO: get back to this. shit's fucked
typedef struct {
  int16_t df_update_ok_voltage;
  // unknown data follows. constant on all dataflash versions, except offset 0x11 on slim
} subclass_0x44; // 68: Configuration: power

// TODO: get back to this. shit's fucked
typedef struct {
    
} subclass_0x50; // 80: Gas gauging: IT Cfg

typedef struct {
    uint16_t dsg_current_threshold;
    uint16_t chg_current_threshold;
    uint16_t quit_current;
    uint16_t dsg_relax_time;
    uint8_t chg_relax_time;
    uint8_t quit_relax_time;
    uint16_t max_ir_correct;
} subclass_0x51; // 81: Gas gauging: Current thresholds

// TODO: get back to this. shit's fucked. on slim it's offset on 1 byte
typedef struct {
    uint16_t qmax; // 0x00 // 0x08B
    uint16_t cycle_count; // 0x02
    uint16_t unk04; // 0x04 Cell0 Chg dod?
    uint8_t unk04; // 0x06
    uint16_t unk07; // 0x07

    int16_t unk07; // 0x09
    int16_t unk07; // 0x0B

    uint16_t unk0D; // 0x0D
    uint16_t unk0F; // 0x0F
    uint16_t unk10; // 0x11
    uint16_t unk12; // 0x13
} subclass_0x52; // 82: Gas gauging: State

// TODO: get back to this. shit's fucked.
typedef struct {
    uint16_t chem_id; // 0x00 // 0x0F9
    uint16_t qmax_cell_0; // 0x02
    // shitton of additional data

} subclass_0x53; // 83: OCV Table: OCVa0 Table

// TODO: get back to this. shit's fucked. doesn't look like OCV table at all
typedef struct {
  // data seems constant
} subclass_0x54; // 84: OCV Table: OCVa1 Table

// TODO: get back to this. shit's fucked.
typedef struct {
 // data seems constant
} subclass_0x55; // 85: Unknown

// TODO: get back to this. shit's fucked. doesn't look like Ra
typedef struct {
} subclass_0x57; // 87: Default Ra:  Def0 Ra

// TODO: get back to this. shit's fucked
typedef struct {
} subclass_0x58; // 88: Default Ra:  Def1 Ra

// TODO: get back to this. shit's fucked. doesn't look like Ra
typedef struct {
} subclass_0x5b; // 91: Ra table:  Pack0 Ra

typedef struct {
} subclass_0x5c; // 91: Ra table:  Pack1 Ra

// TODO: get back to this. shit's fucked. doesn't look like Rax
typedef struct {
} subclass_0x5d; // 93: Ra table:  Pack0 Rax

// TODO: get back to this. shit's fucked. doesn't look like Rax
typedef struct {
} subclass_0x5e; // 93: Ra table:  Pack1 Rax


// TODO: get back to this. seems to match
typedef struct {
    
} subclass_0x68; // 104: Calibration: data

// TODO: get back to this.
typedef struct {
    
} subclass_0x69; // 105: Unknown

// TODO: get back to this. size doesn't match. data somewhat right
typedef struct {
  uint16_t ext_a_coeff_1;
  uint16_t ext_a_coeff_2;
  uint16_t ext_a_coeff_3;
  uint16_t ext_a_coeff_4;
  uint16_t ext_a_coeff_5;
  uint16_t ext_b_coeff_1;
  uint16_t ext_b_coeff_2;
  uint16_t ext_b_coeff_3;
  uint16_t ext_b_coeff_4;
  // unknown data follows
} subclass_0x6a; // 106: Temp Model: data

// TODO: get back to this. size doesn't match
typedef struct {
    
} subclass_0x6b; // 107: Calibration: current

// TODO: get back to this.
typedef struct {
    // continuation of 54 and 55??
} subclass_0x6c; // 108: Unknown

typedef struct {
    uint16_t unseal_code_1; // 0x00 // 0x239
    uint16_t unseal_code_2;
    uint16_t unseal_to_full_1;
    uint16_t unseal_to_full_2;
    uint8_t unknown[16]; // 0123456789ABCDEFFEDCBA9876543210, this string is used on older devices (not 27520) for "secure pairing"
    uint16_t fact_restore_1;
    uint16_t fact_restore_2;
} subclass_0x70; // 112: Security: codes

#pragma pack(pop)