#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <ctype.h>
#include <stdbool.h>

// Define PATH_MAX if not defined
#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

#include "upkg_config.h"
#include "upkg_util.h"

// --- Global Path Variables Definitions ---
char *g_upkg_base_dir = NULL;
char *g_control_dir = NULL;
char *g_db_dir = NULL; // New global variable definition
char *g_install_dir_internal = NULL;
char *g_system_install_root = NULL;

// --- External Global Variables ---
extern bool g_verbose_mode; // Defined in main.c

// --- Internal Function Declarations ---
static void upkg_log_verbose(const char *format, ...);
static void upkg_log_debug(const char *format, ...);

// --- Internal Configuration System Functions ---

/**
 * @brief Logging function for verbose output
 */
static void upkg_log_verbose(const char *format, ...) {
    if (!g_verbose_mode) return;
    
    va_list args;
    va_start(args, format);
    printf("[VERBOSE] ");
    vprintf(format, args);
    va_end(args);
}

/**
 * @brief Logging function for debug output  
 */
static void upkg_log_debug(const char *format, ...) {
    if (!g_verbose_mode) return;
    
    va_list args;
    va_start(args, format);
    printf("[DEBUG] ");
    vprintf(format, args);
    va_end(args);
}

// --- Helper function to find the correct configuration file path ---
char *upkg_get_config_file_path() {
    char *config_file_path = NULL;

    // 1. Check for environment variable override
    char *env_config_path = getenv("UPKG_CONFIG_PATH");
    if (env_config_path && upkg_util_file_exists(env_config_path)) {
        upkg_log_verbose("Using configuration from UPKG_CONFIG_PATH: %s\n", env_config_path);
        config_file_path = strdup(env_config_path);
        if (!config_file_path) {
            upkg_log_debug("Error: Memory allocation failed for config path.\n");
            return NULL;
        }
        return config_file_path;
    }

    // 2. Check for system-wide configuration
    const char *system_config_path = "/etc/upkg/upkgconfig";
    if (upkg_util_file_exists(system_config_path)) {
        upkg_log_verbose("Using system-wide configuration: %s\n", system_config_path);
        config_file_path = strdup(system_config_path);
        if (!config_file_path) {
            upkg_log_debug("Error: Memory allocation failed for config path.\n");
            return NULL;
        }
        return config_file_path;
    }

    // 3. Check for user-specific configuration
    char *home_dir = getenv("HOME");
    if (home_dir) {
        char user_config_path[PATH_MAX];
        snprintf(user_config_path, sizeof(user_config_path), "%s/.upkgconfig", home_dir);
        if (upkg_util_file_exists(user_config_path)) {
            upkg_log_verbose("Using user-specific configuration: %s\n", user_config_path);
            config_file_path = strdup(user_config_path);
            if (!config_file_path) {
                upkg_log_debug("Error: Memory allocation failed for config path.\n");
                return NULL;
            }
            return config_file_path;
        }
    }

    // If no configuration file was found
    upkg_log_debug("Error: No configuration file found.\n");
    upkg_log_debug("Looked for: 1. $UPKG_CONFIG_PATH, 2. /etc/upkg/upkgconfig, 3. ~/.upkgconfig\n");
    return NULL;
}

int load_upkg_config() {
    char *config_file_path = upkg_get_config_file_path();
    if (!config_file_path) {
        // Error message already printed by upkg_get_config_file_path
        return -1;
    }

    // Free any existing global path variables to prevent leaks on re-entry (if applicable)
    upkg_cleanup_paths();

    // Retrieve the directory paths from the determined config file.
    upkg_log_verbose("Loading configuration values from '%s'...\n", config_file_path);
    g_upkg_base_dir = upkg_util_get_config_value(config_file_path, "upkg_dir", '=');
    if (!g_upkg_base_dir) {
        upkg_log_debug("Error: Failed to read 'upkg_dir' from config file. This is critical.\n");
        upkg_util_free_and_null(&config_file_path);
        upkg_cleanup_paths(); // Clean up anything partially allocated
        return -1;
    }

    g_control_dir = upkg_util_get_config_value(config_file_path, "control_dir", '=');
    if (!g_control_dir) {
        upkg_log_debug("Error: Failed to read 'control_dir' from config file. This is critical.\n");
        upkg_util_free_and_null(&config_file_path);
        upkg_cleanup_paths();
        return -1;
    }

    g_db_dir = upkg_util_get_config_value(config_file_path, "db_dir", '='); // New value
    if (!g_db_dir) {
        upkg_log_debug("Error: Failed to read 'db_dir' from config file. This is critical.\n");
        upkg_util_free_and_null(&config_file_path);
        upkg_cleanup_paths();
        return -1;
    }

    g_install_dir_internal = upkg_util_get_config_value(config_file_path, "install_dir", '=');
    if (!g_install_dir_internal) {
        upkg_log_debug("Error: Failed to read 'install_dir' from config file. This is critical.\n");
        upkg_util_free_and_null(&config_file_path);
        upkg_cleanup_paths();
        return -1;
    }

    // Assign g_system_install_root from install_dir config value.
    g_system_install_root = strdup(g_install_dir_internal);
    if (!g_system_install_root) {
        upkg_log_debug("Error: Failed to duplicate 'install_dir' for g_system_install_root.\n");
        upkg_util_free_and_null(&config_file_path);
        upkg_cleanup_paths();
        return -1;
    }
    
    upkg_util_free_and_null(&config_file_path); // Free the path string after use

    upkg_log_verbose("Configuration loaded successfully:\n");
    upkg_log_verbose("  upkg_base_dir: %s\n", g_upkg_base_dir);
    upkg_log_verbose("  control_dir: %s\n", g_control_dir);
    upkg_log_verbose("  db_dir: %s\n", g_db_dir); // New log message
    upkg_log_verbose("  install_dir_internal (record keeping): %s\n", g_install_dir_internal);
    upkg_log_verbose("  system_install_root (actual target): %s\n", g_system_install_root);

    return 0;
}

void upkg_cleanup_paths() {
    upkg_log_verbose("Cleaning up global path variables...\n");
    upkg_util_free_and_null(&g_upkg_base_dir);
    upkg_util_free_and_null(&g_control_dir);
    upkg_util_free_and_null(&g_db_dir); // New cleanup call
    upkg_util_free_and_null(&g_install_dir_internal);
    upkg_util_free_and_null(&g_system_install_root);
}

void upkg_init_paths() {
    // NEW LOGIC: Load paths from upkgconfig
    upkg_log_verbose("Initializing upkg paths from config...\n");
    if (load_upkg_config() != 0) {
        upkg_log_debug("Error: Failed to load upkg configuration. Exiting.\n");
        exit(EXIT_FAILURE);
    }

    // Now, create the directories based on the loaded config paths
    // Check for NULL pointers before calling create_dir_recursive
    if (!g_upkg_base_dir || !g_control_dir || !g_db_dir || !g_install_dir_internal) {
        upkg_log_debug("Error: One or more critical path variables are NULL after config load. Cannot create directories. Exiting.\n");
        upkg_cleanup_paths(); // Clean up anything that might have been allocated
        exit(EXIT_FAILURE);
    }

    upkg_log_verbose("Creating necessary upkg directories...\n");
    if (upkg_util_create_dir_recursive(g_control_dir, 0755) != 0 ||
        upkg_util_create_dir_recursive(g_db_dir, 0755) != 0 || // New directory creation
        upkg_util_create_dir_recursive(g_install_dir_internal, 0755) != 0) {
        upkg_log_debug("Error: Failed to create necessary upkg directories based on config. Exiting.\n");
        upkg_cleanup_paths();
        exit(EXIT_FAILURE);
    }

    upkg_log_verbose("upkg directories initialized from config:\n");
    upkg_log_verbose("  Base: %s\n", g_upkg_base_dir);
    upkg_log_verbose("  Control: %s\n", g_control_dir);
    upkg_log_verbose("  Database: %s\n", g_db_dir); // New log message
    upkg_log_verbose("  Internal Install Records: %s\n", g_install_dir_internal);
    upkg_log_verbose("  System Root (actual install target): %s\n", g_system_install_root);
}
