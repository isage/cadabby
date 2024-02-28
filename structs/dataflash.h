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
  int16_t fast_charge_current; // unused?
  int16_t charging_voltage;
  int16_t delta_temp;
  int16_t suspend_low_temp;
  int16_t suspend_high_temp;
} subclass_0x22; // 34: Configuration: Charge

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
    uint16_t soc_set_threshold; // 0x00
    uint16_t soc_clear_threshold; // 0x02
    uint8_t max_load_rsoc; // 0x03
//
    uint16_t final_voltage; // 0x0F
} subclass_0x31; // 49: Configuration: Discharge

typedef struct {
    uint8_t full_reset_counter; // 0x00 // 0x26A
    uint8_t watchdog_reset_counter; // 0x01
    uint32_t firmware_checksum; // 0x02
    uint8_t df_restore_counter; // maybe? or df_checksum? zeroed-out anyway
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
  int16_t sleep_current;
  int8_t bus_low_time;
  int16_t cal_inhibit_temp_low;
  int16_t cal_inhibit_temp_high;
  uint8_t sleep_voltage_time;
  uint8_t sleep_current_time;
  uint16_t hibernate_current;
  // unknown data follows. constant on all dataflash versions, except offset 0x11 on slim
} subclass_0x44; // 68: Configuration: power

// TODO: get back to this. shit's fucked
typedef struct {
    uint8_t load_select;
    uint8_t load_mode;
    uint16_t max_dicharge_duration_max;
    uint16_t max_dicharge_duration_min;
    uint16_t secondary_relax_time;
    uint16_t resistance_data_interval;
    uint8_t level_margin;
    uint8_t min_level_time;
    uint8_t sample_avg_relax_time;
    uint16_t ocv_wait_time;
    uint16_t remcap_update_time;
    uint16_t res_current;
    uint8_t min_dod_res_update;
    uint8_t dod_capacity_err;

    uint8_t unk1; // 1e, ti: max_res_factor: 0f
    uint8_t unk2; // 03, ti: min_res_factor: 07
    uint8_t unk3; // 0f, ti: numbergrid_res_scale: 0f
    uint8_t unk4; // 0a, ti: unk

    uint16_t ra_filter;
    uint16_t samples_to_wake;
    uint16_t qmax_max_time;
    uint8_t max_v_delta;
    uint16_t q_invalid_max_v;
    uint16_t q_invalid_min_v;
    uint16_t q_invalid_max_t;
    uint16_t q_invalid_min_t;
    uint8_t dod_validation_time;

    uint8_t min_percent_passed_chg_for_qmax; // Minimum % Passed Charge for Qmax DEFAULT:  0x0025
    uint8_t min_percent_passed_chg_for_1st_qmax; // Minimum % Passed Charge for 1st Qmax DEFAULT:  0x005A
    uint8_t qmax_capacity_err; // Q Maximum Capacity Error DEFAULT:  0x000F
    uint8_t max_qmax_change; // Maximum Qmax Change DEFAULT:  0x0014
    uint8_t qmax_filter; // Qmax Update Filter DEFAULT:  0x0060
    uint16_t terminate_voltage;
    uint16_t terminate_voltage_delta;
    uint8_t simulation_qinc;
    uint16_t res_relax_time;
    uint16_t outside_temp_time;

    uint16_t unk0x35[9]; // user rate and stuff, but more than there is in struct

    uint16_t ocv_wait_time_after_discharge;
    uint8_t max_scale_back_grid;
    uint8_t max_sim_rate;
    uint8_t min_sim_rate;

    uint16_t ra_max_delta; // maybe?
    uint16_t unk_0x50[2];

    uint16_t max_res_scale;
    uint16_t min_res_scale;
    uint8_t fast_scale_start_soc;
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

// TODO: get back to this. shit's fucked
typedef struct {
    uint16_t qmax; // 0x00 // 0x08B
    uint16_t cycle_count; // 0x02

    uint16_t unk04; // 0x04
    uint8_t unk04; // 0x06
    uint16_t unk07; // 0x07


    int16_t avg_current_last_run; // 0x09
    int16_t avg_power_last_run; // 0x0B
    int16_t pulse_delta_voltage; // 0x0D
    uint16_t max_discharge_duration; // 0x0F

    uint16_t unk11; // 0x11
    uint16_t unk13; // 0x13

} subclass_0x52; // 82: Gas gauging: State

// TODO: get back to this. shit's fucked
typedef struct {
    uint8_t it_enable; // 0x00 // 0x08B
    uint16_t qmax; // 0x00 // 0x08B
    uint16_t cycle_count; // 0x02

    uint16_t unk04; // 0x04
    uint8_t unk04; // 0x06
    uint16_t unk07; // 0x07


    int16_t avg_current_last_run; // 0x09
    int16_t avg_power_last_run; // 0x0B
    int16_t pulse_delta_voltage; // 0x0D
    uint16_t max_discharge_duration; // 0x0F

    uint16_t unk11; // 0x11
    uint16_t unk13; // 0x13

} subclass_0x52_slim; // 82: Gas gauging: State

// TODO: get back to this. shit's fucked.
typedef struct {
    uint16_t chem_id; // 0x00 // 0x0F9
    uint16_t qmax_cell_0; // 0x02
    // shitton of additional data
    uint16_t unk[4];
    // voltage table follows?

} subclass_0x53; // 83: OCV Table: OCVa0 Table

// TODO: get back to this. shit's fucked. doesn't look like OCV table at all. looks like voltage table from 0x53
typedef struct {
  // data seems constant
} subclass_0x54; // 84: OCV Table: OCVa1 Table

// TODO: get back to this. shit's fucked. looks like voltage table from 0x53
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


// TODO: first two fields are "float"
typedef struct {
  uint16_t cc_gain; // CC Sense Resistor Gain DEFAULT:  0x0000
  uint16_t cc_delta; // Coulomb Counter Delta DEFAULT:  0x8A9C0
  int16_t cc_offset; // Coulomb Counter Offset DEFAULT:  0xFFFFFFFFFFFFFB50
  int16_t adc_offset; // ADC Current Offset DEFAULT:  0x004C
  int8_t board_offset; // Board Offset DEFAULT:  0x0000
  int8_t int_temp_offset; // Internal Temperature Offset DEFAULT:  0x0000
  int8_t ext_temp_offset; // External Temperature Offset DEFAULT:  0x0000
  int8_t pack_voltage_offset; // Pack Voltage Offset DEFAULT:  0x0000
} subclass_0x68; // 104: Calibration: data

typedef struct {
  int16_t cc_current; // Coulomb Counter Current - unused DEFAULT:  0x0BB8
  int16_t voltage_signal; // Voltage Signal - unused DEFAULT:  0x1068
  int16_t temp_signal; // Temperature Signal - unused DEFAULT:  0x0BA4
  uint16_t cc_offset_time; // Coulomb Counter Offset Time - unused DEFAULT:  0x03E8
  uint16_t adc_offset_time; // ADC Offset Time - unused DEFAULT:  0x0020
  uint16_t cc_gain_time; // Coulomb Counter Gain Time - unused DEFAULT:  0x03E8
  uint16_t voltage_time; // Voltage Time - unused DEFAULT:  0x0020
  uint16_t temperature_time; // Temperature Time - unused DEFAULT:  0x0020
  uint8_t afe_gain_readings; // AFE Gain Readings - unused DEFAULT:  0x0010
  uint16_t cal_mode_timeout; // Cal Mode Timeout - unused DEFAULT:  0x9600
} subclass_0x69; // 105: Calibration config

// TODO: get back to this. size doesn't match. data somewhat right
typedef struct {
  int16_t ext_a_coeff_1;
  int16_t ext_a_coeff_2;
  int16_t ext_a_coeff_3;
  int16_t ext_a_coeff_4;
  int16_t ext_a_coeff_5;
  int16_t ext_b_coeff_1;
  int16_t ext_b_coeff_2;
  int16_t ext_b_coeff_3;
  int16_t ext_b_coeff_4;
  int16_t ext_ideal_ad;
  int16_t ext_true_ad;
  int16_t int_coef_1; // Int Coefficient 1 DEFAULT:  0x0000
  int16_t int_coef_2; // Int Coefficient 2 DEFAULT:  0x0000
  int16_t int_coef_3; // Int Coefficient 3 DEFAULT:  0xFFFFFFFFFFFFCFDC
  int16_t int_coef_4; // Int Coefficient 4 DEFAULT:  0x17F3
  int16_t int_min_ad; // Int Minimum AD DEFAULT:  0x0000
  int16_t int_max_temp; // Int Maximum Temperature DEFAULT:  0x17F3
  int16_t vcomp_coef_1; // Vcomp Coefficient 1 DEFAULT:  0x0000
  int16_t vcomp_coef_2; // Vcomp Coefficient 2 DEFAULT:  0x3A36
  int16_t vcomp_coef_3; // Vcomp Coefficient 3 DEFAULT:  0xFFFFFFFFFFFFFD91
  int16_t vcomp_coef_4; // Vcomp Coefficient 4 DEFAULT:  0x0025
  uint8_t vcomp_input_multiplier; // Vcomp Input Multiplier DEFAULT:  0x0030
  int16_t vcomp_output_divisor; // Vcomp Output Divisor DEFAULT:  0x0100
  // unknown data follows, atleast 5 bytes
} subclass_0x6a; // 106: Temp Model: data

typedef struct {
  uint8_t filter; // Filter DEFAULT:  0x00EF
  uint8_t deadband; // Deadband DEFAULT:  0x0005
  uint8_t cc_deadband; // Coulomb Counter Deadband DEFAULT:  0x0011
  uint8_t cc_max_deadband; // Coulomb Counter Maximum Deadband DEFAULT:  0x0005
  uint16_t cc_deadband_sample; // Coulomb Counter Deadband Samples DEFAULT:  0x0100
  uint16_t cc_max_offset_sample; // Coulomb Counter Offset Samples DEFAULT:  0x0010
  int16_t dv_lim; // Dynamic Offset Delta Voltage Limit DEFAULT:  0x0100
  uint8_t dv_time; // Dynamic Offset Delta Voltage Limit Delay DEFAULT:  0x003C
  int16_t dt_lim; // Dynamic Offset Temperature Limit DEFAULT:  0x0050
  uint8_t dt_time; // Dynamic Offset Delta Temperature Limit Delay DEFAULT:  0x003C
  int16_t cur_lim; // Dynamic Offset Current Limit DEFAULT:  0x0064
  uint8_t cur_time; // Dynamic Offset Current Limit Delay DEFAULT:  0x003C
  int16_t vd_lim; // Dynamic Offset Voltage Drop Limit DEFAULT:  0x0020
} subclass_0x6b; // 107: Calibration: current

// TODO: get back to this.
typedef struct {
    // continuation of 54 and 55??
} subclass_0x6c; // 108: Rb0 Lo table

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