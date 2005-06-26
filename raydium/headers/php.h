#ifndef _PHP_H
#define _PHP_H
// PHP support for Raydium
// Known bug: recursive Ray/PHP calls are fatal (segfault in zend core).

#include "../php_wrappers.c"
// use this macro when registering your functions
#define C2PHP ZEND_FN


// Dirty globals... (needed for WIN32 PHP support)
#ifdef ZTS
extern zend_compiler_globals *compiler_globals;
extern zend_executor_globals *executor_globals;
extern php_core_globals *core_globals;
extern sapi_globals_struct *sapi_globals;
extern void ***tsrm_ls;
#endif
extern int raydium_init_cli_option (char *option, char *value);;
// { // Unrecognized
//   return SAPI_HEADER_SENT_SUCCESSFULLY; // Unrecognized
// } // Unrecognized
// { // Unrecognized
// } // Unrecognized
// { // Unrecognized
//   return SUCCESS; // Unrecognized
// } // Unrecognized
extern void raydium_php_error (int type, const char *msg, ...);
extern int raydium_php_uwrite (const char *str, uint str_length TSRMLS_DC);
//   "RayHandler", // Unrecognized
//   "Raydium PHP Handler", // Unrecognized
//   php_dummy, // Unrecognized
//   php_dummy, // Unrecognized
//   NULL, // Unrecognized
//   NULL, // Unrecognized
//   raydium_php_uwrite, // Unrecognized
//   NULL, // Unrecognized
//   NULL, // Unrecognized
//   NULL, // Unrecognized
//   raydium_php_error, // Unrecognized
//   NULL, // Unrecognized
//   sapi_raydium_send_headers, // Unrecognized
//   sapi_raydium_send_header, // Unrecognized
//   NULL, // Unrecognized
//   NULL, // Unrecognized
//   NULL, // Unrecognized
//   NULL, // Unrecognized
//   NULL, // Unrecognized
//   NULL, // Unrecognized
//   NULL, // Unrecognized
//   STANDARD_SAPI_MODULE_PROPERTIES // Unrecognized
// }; // Unrecognized
extern void raydium_php_init_request (char *filename);
extern int raydium_php_exec (char *name);
extern void raydium_php_close (void);
extern void raydium_php_init (void);
#endif
