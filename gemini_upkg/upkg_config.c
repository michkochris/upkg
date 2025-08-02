#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>

#include "upkg_config.h"
#include "upkg_lib.h"
#include "upkg_hash.h" // Includes our new logging function prototypes

// --- Global Path Variables Definitions ---
char *g_upkg_base_dir = NULL;
char *g_control_dir = NULL;
char *g_unpack_dir = NULL;
char *g_db_dir = NULL; // New global variable definition
char *g_install_dir_internal = NULL;
char *g_system_install_root = NULL;

// --- Helper function to find the correct configuration file path ---
static char *get_config_path() {
    char *config_file_path = NULL;

    // 1. Check for environment variable override
    char *env_config_path = getenv("UPKG_CONFIG_PATH");
    if (env_config_path && file_exists(env_config_path)) {
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
    if (file_exists(system_config_path)) {
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
        if (file_exists(user_config_path)) {
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
    char *config_file_path = get_config_path();
    if (!config_file_path) {
        // Error message already printed by get_config_path
        return -1;
    }

    // Free any existing global path variables to prevent leaks on re-entry (if applicable)
    upkg_cleanup_paths();

    // Retrieve the directory paths from the determined config file.
    upkg_log_verbose("Loading configuration values from '%s'...\n", config_file_path);
    g_upkg_base_dir = get_config_value(config_file_path, "upkg_dir", '=');
    if (!g_upkg_base_dir) {
        upkg_log_debug("Error: Failed to read 'upkg_dir' from config file. This is critical.\n");
        free_and_null(&config_file_path);
        upkg_cleanup_paths(); // Clean up anything partially allocated
        return -1;
    }

    g_control_dir = get_config_value(config_file_path, "control_dir", '=');
    if (!g_control_dir) {
        upkg_log_debug("Error: Failed to read 'control_dir' from config file. This is critical.\n");
        free_and_null(&config_file_path);
        upkg_cleanup_paths();
        return -1;
    }

    g_unpack_dir = get_config_value(config_file_path, "unpack_dir", '=');
    if (!g_unpack_dir) {
        upkg_log_debug("Error: Failed to read 'unpack_dir' from config file. This is critical.\n");
        free_and_null(&config_file_path);
        upkg_cleanup_paths();
        return -1;
    }

    g_db_dir = get_config_value(config_file_path, "db_dir", '='); // New value
    if (!g_db_dir) {
        upkg_log_debug("Error: Failed to read 'db_dir' from config file. This is critical.\n");
        free_and_null(&config_file_path);
        upkg_cleanup_paths();
        return -1;
    }

    g_install_dir_internal = get_config_value(config_file_path, "install_dir", '=');
    if (!g_install_dir_internal) {
        upkg_log_debug("Error: Failed to read 'install_dir' from config file. This is critical.\n");
        free_and_null(&config_file_path);
        upkg_cleanup_paths();
        return -1;
    }

    // Assign g_system_install_root from install_dir config value.
    g_system_install_root = strdup(g_install_dir_internal);
    if (!g_system_install_root) {
        upkg_log_debug("Error: Failed to duplicate 'install_dir' for g_system_install_root.\n");
        free_and_null(&config_file_path);
        upkg_cleanup_paths();
        return -1;
    }
    
    free_and_null(&config_file_path); // Free the path string after use

    upkg_log_verbose("Configuration loaded successfully:\n");
    upkg_log_verbose("  upkg_base_dir: %s\n", g_upkg_base_dir);
    upkg_log_verbose("  control_dir: %s\n", g_control_dir);
    upkg_log_verbose("  unpack_dir: %s\n", g_unpack_dir);
    upkg_log_verbose("  db_dir: %s\n", g_db_dir); // New log message
    upkg_log_verbose("  install_dir_internal (record keeping): %s\n", g_install_dir_internal);
    upkg_log_verbose("  system_install_root (actual target): %s\n", g_system_install_root);

    return 0;
}

void upkg_cleanup_paths() {
    upkg_log_verbose("Cleaning up global path variables...\n");
    free_and_null(&g_upkg_base_dir);
    free_and_null(&g_control_dir);
    free_and_null(&g_unpack_dir);
    free_and_null(&g_db_dir); // New cleanup call
    free_and_null(&g_install_dir_internal);
    free_and_null(&g_system_install_root);
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
    if (!g_upkg_base_dir || !g_control_dir || !g_unpack_dir || !g_db_dir || !g_install_dir_internal) {
        upkg_log_debug("Error: One or more critical path variables are NULL after config load. Cannot create directories. Exiting.\n");
        upkg_cleanup_paths(); // Clean up anything that might have been allocated
        exit(EXIT_FAILURE);
    }

    upkg_log_verbose("Creating necessary upkg directories...\n");
    if (create_dir_recursive(g_control_dir, 0755) != 0 ||
        create_dir_recursive(g_unpack_dir, 0755) != 0 ||
        create_dir_recursive(g_db_dir, 0755) != 0 || // New directory creation
        create_dir_recursive(g_install_dir_internal, 0755) != 0) {
        upkg_log_debug("Error: Failed to create necessary upkg directories based on config. Exiting.\n");
        upkg_cleanup_paths();
        exit(EXIT_FAILURE);
    }

    upkg_log_verbose("upkg directories initialized from config:\n");
    upkg_log_verbose("  Base: %s\n", g_upkg_base_dir);
    upkg_log_verbose("  Control: %s\n", g_control_dir);
    upkg_log_verbose("  Unpack: %s\n", g_unpack_dir);
    upkg_log_verbose("  Database: %s\n", g_db_dir); // New log message
    upkg_log_verbose("  Internal Install Records: %s\n", g_install_dir_internal);
    upkg_log_verbose("  System Root (actual install target): %s\n", g_system_install_root);
}
