/* Copyright (C) 2009 - 2022 National Aeronautics and Space Administration.
   All Foreign Rights are Reserved to the U.S. Government.

   This software is provided "as is" without any warranty of any kind, either expressed, implied, or statutory,
   including, but not limited to, any warranty that the software will conform to specifications, any implied warranties
   of merchantability, fitness for a particular purpose, and freedom from infringement, and any warranty that the
   documentation will conform to the program, or any warranty that the software will be error free.

   In no event shall NASA be liable for any damages, including, but not limited to direct, indirect, special or
   consequential damages, arising out of, resulting from, or in any way connected with the software or its
   documentation, whether or not based upon warranty, contract, tort or otherwise, and whether or not loss was sustained
   from, or arose out of the results of, or use of, the software, documentation or services provided hereunder.

   ITC Team
   NASA IV&V
   jstar-development-team@mail.nasa.gov
*/

#ifndef _crypto_h_
#define _crypto_h_

/*
** Crypto Includes
*/
#ifdef NOS3 // NOS3/cFS build is ready
#include "cfe.h"
#else // Assume build outside of NOS3/cFS infrastructure
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#endif

#include "crypto_config.h"
#include "crypto_config_structs.h"
#include "crypto_error.h"
#include "crypto_events.h"
#include "crypto_print.h"
#include "crypto_structs.h"
#include "sadb_routine.h"
#include "cryptography_interface.h"

/*
** Crypto Version
*/
#define CRYPTO_LIB_MAJOR_VERSION 1
#define CRYPTO_LIB_MINOR_VERSION 0
#define CRYPTO_LIB_REVISION 1
#define CRYPTO_LIB_MISSION_REV 0

/*
** User Prototypes
*/

// Crypto Library Configuration functions
extern int32_t Crypto_Config_CryptoLib(uint8_t sadb_type, uint8_t cryptography_type, uint8_t crypto_create_fecf, uint8_t process_sdls_pdus,
                                       uint8_t has_pus_hdr, uint8_t ignore_sa_state, uint8_t ignore_anti_replay,
                                       uint8_t unique_sa_per_mapid, uint8_t crypto_check_fecf, uint8_t vcid_bitmask);
/*===========================================================================
Function:           Crypto_Config_MariaDB
Description:        sets the fields the struct SadbMariaDBConfig_t for required 
 *                  parameters to create MySQL connection. 
 *                  1) char* mysql_username -  mariadb username
 *                  2) char* mysql_password - password associated with the username
 *                  3) char* mysql_hostname - hostname of the server that hosts the mariadb database
 *                  4) char* mysql_hostname - database schema name - OPTIONAL. 
                    5) char* mysql_hostname - port associated with mariadb. By default port 3306. 
                    6) uint8_t encrypted_connection - attempting an encrypted connection. 
 *                  Set encrypted_connection = 1 if you are attempting an encrypted connection. 
                    Optional parameters that are only required for an encrypted connection:
                    uint8_t encrypted_connection
                    7) char* ssl_cert - The path name of the server public key certificate file with .pem extension. 
                    8) char* ssl_key - The path name of the server private key file with .pem extension. 
                    9) char* ssl_ca - The path name of the Certificate Authority (CA) certificate file. 
                    10) char* ssl_capath - Certificate Authority (CA) directory.      
Outputs:            status - int32
References:         1) https://dev.mysql.com/doc/c-api/8.0/en/c-api-encrypted-
 *                      connections.html#c-api-enforcing-encrypted-connection
 *                  2) https://dev.mysql.com/doc/c-api/8.0/en/mysql-ssl-set.html
 *                  3) https://www.xuchao.org/docs/mysql/connectors-apis.html#c-api-encrypted-connections
Example call:        
Note:               MySQL server MUST be configured for encrypted connections:
 *                  https://dev.mysql.com/doc/refman/5.7/en/using-encrypted-connections.html
==========================================================*/
extern int32_t Crypto_Config_MariaDB(char* mysql_username, char* mysql_password, char* mysql_hostname, char* mysql_database, uint16_t mysql_port, uint8_t encrypted_connection, char* ssl_cert, char* ssl_key, char* ssl_ca, char* ssl_capath,uint8_t mariadb_tls_verify_server, char* mtls_client_key_password);
extern int32_t Crypto_Config_Kmc_Crypto_Service(char *protocol, char *kmc_crypto_hostname, uint16_t kmc_crypto_port, char *kmc_crypto_app_uri, char *mtls_client_cert_path, char *mtls_client_cert_type,
                                                char *mtls_client_key_path,char *mtls_client_key_pass, char *mtls_ca_bundle, char *mtls_ca_path,
                                                char *mtls_issuer_cert, uint8_t ignore_ssl_hostname_validation);
extern int32_t Crypto_Config_Add_Gvcid_Managed_Parameter(uint8_t tfvn, uint16_t scid, uint8_t vcid, uint8_t has_fecf,
                                                         uint8_t has_segmentation_hdr);

// Initialization
extern int32_t Crypto_Init(void); // Initialize CryptoLib After Configuration Calls
extern int32_t Crypto_Init_With_Configs(
    CryptoConfig_t *crypto_config_p, GvcidManagedParameters_t *gvcid_managed_parameters_p,
    SadbMariaDBConfig_t *sadb_mariadb_config_p,
    CryptographyKmcCryptoServiceConfig_t *cryptography_kmc_crypto_config_p); // Initialize CryptoLib With Application Defined Configuration
extern int32_t Crypto_Init_Unit_Test(void);      // Initialize CryptoLib with unit test default Configurations

// Cleanup
extern int32_t Crypto_Shutdown(void); // Free all allocated memory

// Telecommand (TC)
extern int32_t Crypto_TC_ApplySecurity(const uint8_t *p_in_frame, const uint16_t in_frame_length,
                                       uint8_t **pp_enc_frame, uint16_t *p_enc_frame_len);
extern int32_t Crypto_TC_ProcessSecurity(uint8_t *ingest, int *len_ingest, TC_t *tc_sdls_processed_frame);
// Telemetry (TM)
extern int32_t Crypto_TM_ApplySecurity(uint8_t *ingest, int *len_ingest);
extern int32_t Crypto_TM_ProcessSecurity(uint8_t *ingest, int *len_ingest);
// Advanced Orbiting Systems (AOS)
extern int32_t Crypto_AOS_ApplySecurity(uint8_t *ingest, int *len_ingest);
extern int32_t Crypto_AOS_ProcessSecurity(uint8_t *ingest, int *len_ingest);

/*
** Internal Prototypes
*/
extern uint8_t Crypto_Prep_Reply(uint8_t *ingest, uint8_t appID);
extern int32_t Crypto_increment(uint8_t *num, int length);
// int32_t  Crypto_Get_tcPayloadLength(TC_t* tc_frame, SecurityAssociation_t *sa_ptr);
int32_t Crypto_Get_tmLength(int len);
uint8_t Crypto_Is_AEAD_Algorithm(uint32_t cipher_suite_id);
uint8_t *Crypto_Prepare_TC_AAD(uint8_t *buffer, uint16_t len_aad, uint8_t *abm_buffer);
void Crypto_TM_updatePDU(uint8_t *ingest, int len_ingest);
void Crypto_TM_updateOCF(void);
void Crypto_Local_Config(void);
void Crypto_Local_Init(void);
// int32_t  Crypto_gcm_err(int gcm_err);
int32_t Crypto_window(uint8_t *actual, uint8_t *expected, int length, int window);
// int32_t Crypto_compare_less_equal(uint8_t *actual, uint8_t *expected, int length);
// int32_t  Crypto_FECF(int fecf, uint8_t* ingest, int len_ingest,TC_t* tc_frame);
uint16_t Crypto_Calc_FECF(uint8_t *ingest, int len_ingest);
void Crypto_Calc_CRC_Init_Table(void);
uint16_t Crypto_Calc_CRC16(uint8_t *data, int size);

// Key Management Functions
int32_t Crypto_Key_OTAR(void);
int32_t Crypto_Key_update(uint8_t state);
int32_t Crypto_Key_inventory(uint8_t *);
int32_t Crypto_Key_verify(uint8_t *, TC_t *tc_frame);

// Security Monitoring & Control Procedure
int32_t Crypto_MC_ping(uint8_t *ingest);
int32_t Crypto_MC_status(uint8_t *ingest);
int32_t Crypto_MC_dump(uint8_t *ingest);
int32_t Crypto_MC_erase(uint8_t *ingest);
int32_t Crypto_MC_selftest(uint8_t *ingest);
int32_t Crypto_SA_readARSN(uint8_t *ingest);
int32_t Crypto_MC_resetalarm(void);

// User Functions
int32_t Crypto_User_IdleTrigger(uint8_t *ingest);
int32_t Crypto_User_BadSPI(void);
int32_t Crypto_User_BadIV(void);
int32_t Crypto_User_BadMAC(void);
int32_t Crypto_User_BadFECF(void);
int32_t Crypto_User_ModifyKey(void);
int32_t Crypto_User_ModifyActiveTM(void);
int32_t Crypto_User_ModifyVCID(void);

// Determine Payload Data Unit
int32_t Crypto_Process_Extended_Procedure_Pdu(TC_t *tc_sdls_processed_frame, uint8_t *ingest);
int32_t Crypto_PDU(uint8_t *ingest, TC_t *tc_frame);

// Managed Parameter Functions
int32_t Crypto_Get_Managed_Parameters_For_Gvcid(uint8_t tfvn, uint16_t scid, uint8_t vcid,
                                                       GvcidManagedParameters_t *managed_parameters_in,
                                                       GvcidManagedParameters_t **managed_parameters_out);
int32_t crypto_config_add_gvcid_managed_parameter_recursion(uint8_t tfvn, uint16_t scid, uint8_t vcid,
                                                                   uint8_t has_fecf, uint8_t has_segmentation_hdr,
                                                                   GvcidManagedParameters_t *managed_parameter);
void Crypto_Free_Managed_Parameters(GvcidManagedParameters_t *managed_parameters);



/*
** Extern Global Variables
*/ 
// Data stores used in multiple components
extern CCSDS_t sdls_frame;
extern TM_t tm_frame;

// Global configuration structs
extern CryptoConfig_t *crypto_config;
extern SadbMariaDBConfig_t *sadb_mariadb_config;
extern CryptographyKmcCryptoServiceConfig_t *cryptography_kmc_crypto_config;
extern GvcidManagedParameters_t *gvcid_managed_parameters;
extern GvcidManagedParameters_t *current_managed_parameters;
extern SadbRoutine sadb_routine;
extern CryptographyInterface cryptography_if;

// extern crypto_key_t ak_ring[NUM_KEYS];
extern CCSDS_t sdls_frame;
extern TM_t tm_frame;
extern CryptoConfig_t *crypto_config;
extern SadbMariaDBConfig_t *sadb_mariadb_config;
extern GvcidManagedParameters_t *gvcid_managed_parameters;
extern GvcidManagedParameters_t *current_managed_parameters;
// OCF
extern uint8_t ocf;
extern SDLS_FSR_t report;
extern TM_FrameCLCW_t clcw;
// Flags
extern SDLS_MC_LOG_RPLY_t log_summary;
extern SDLS_MC_DUMP_BLK_RPLY_t mc_log;
extern uint8_t log_count;
extern uint16_t tm_offset;
// ESA Testing - 0 = disabled, 1 = enabled
extern uint8_t badSPI;
extern uint8_t badIV;
extern uint8_t badMAC;
extern uint8_t badFECF;
//  CRC
extern uint32_t crc32Table[256];
extern uint16_t crc16Table[256];

#endif // _crypto_h_