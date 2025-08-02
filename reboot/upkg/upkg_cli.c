#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>

#include "upkg_config.h"
#include "upkg_pack.h"
#include "upkg_hash.h"

// Global variables
bool g_verbose_mode = false;

// --- Simple Logging Functions ---

/**
 * @brief Logging function for verbose output
 */
void upkg_log_verbose(const char *format, ...) {
    if (!g_verbose_mode) return;

    va_list args;
    va_start(args, format);
    printf("[VERBOSE] ");
    vprintf(format, args);
    va_end(args);
}

// --- Placeholder Function Implementations ---

/**
 * @brief Prints the program's usage information.
 */
void usage(void) {
    printf("upkg - The ulinux package manager.\n\n");
    printf("Usage:\n");
    printf("  upkg <COMMAND> [OPTIONS] [ARGUMENTS]\n\n");
    printf("Commands and Options:\n");
    printf("  -i, --install <path-to-package.deb>...  Install one or more .deb files.\n");
    printf("  -r, --remove <package-name>             Remove a package.\n");
    printf("  -l, --list                              List all installed packages.\n");
    printf("  -s, --status <package-name>             Show detailed information about a package.\n");
    printf("  -S, --search <query>                    Search for a package by name.\n");
    printf("  -v, --verbose                           Enable verbose output.\n");
    printf("      --version                           Print version information.\n");
    printf("  -h, --help                              Display this help message.\n\n");
    printf("      --print-config                      Print current configuration settings.\n");
    printf("      --print-config-file                 Print path to configuration file in use.\n");
    printf("Note: Commands can be interleaved, e.g., 'upkg -v -i pkg1.deb -s pkg2 -i pkg3.deb'\n");
}

/**
 * @brief Prints version information.
 */
void handle_version(void) {
    printf("upkg v0.1.0 - The ulinux package manager\n");
    printf("Copyright (c) 2025 upkg (ulinux) All rights reserved.\n");
    printf("Licensed under GPL v3\n");
}

/**
 * @brief Initializes the upkg environment with configuration loading.
 * @return 0 on success, -1 on failure.
 */
int upkg_init(void) {
    upkg_log_verbose("Initializing upkg environment...\n");
    
    // Initialize paths from configuration file
    upkg_init_paths();
    
    upkg_log_verbose("upkg environment initialized successfully.\n");
    return 0; // Success
}

/**
 * @brief Cleans up upkg environment and frees allocated memory.
 */
void upkg_cleanup(void) {
    upkg_log_verbose("Cleaning up upkg environment...\n");
    
    // Clean up hash table if it exists
    if (upkg_main_hash_table) {
        upkg_hash_destroy_table(upkg_main_hash_table);
        upkg_main_hash_table = NULL;
    }
    
    // Clean up configuration paths
    upkg_cleanup_paths();
    
    upkg_log_verbose("upkg cleanup completed.\n");
}

/**
 * @brief Prints error messages with formatting support.
 */
void errormsg(const char *format, ...) {
    va_list args;
    va_start(args, format);
    
    fprintf(stderr, "ERROR: ");
    vfprintf(stderr, format, args);
    
    va_end(args);
}

/**
 * @brief Handles package installation with info collection and display.
 */
void handle_install(const char *deb_file_path) {
    upkg_log_verbose("Installing package from: %s\n", deb_file_path);
    printf("Installing package from: %s\n", deb_file_path);
    
    if (!g_control_dir) {
        printf("Error: Control directory not configured. Please check your upkg configuration.\n");
        return;
    }
    
    // Initialize package info structure
    upkg_package_info_t pkg_info;
    upkg_pack_init_package_info(&pkg_info);
    
    // Extract package and collect information
    printf("\nExtracting package and collecting information...\n");
    int result = upkg_pack_extract_and_collect_info(deb_file_path, g_control_dir, &pkg_info);
    
    if (result == 0) {
        printf("Package extraction successful!\n\n");
        
        // Print the collected package information
        upkg_pack_print_package_info(&pkg_info);
        
        // Initialize hash table if not already created
        if (!upkg_main_hash_table) {
            upkg_main_hash_table = upkg_hash_create_table(INITIAL_HASH_TABLE_SIZE);
            if (!upkg_main_hash_table) {
                printf("Warning: Failed to create hash table for package management.\n");
            } else {
                upkg_log_verbose("Hash table initialized for package management.\n");
            }
        }
        
        // Add package to hash table if table exists
        if (upkg_main_hash_table) {
            upkg_hash_package_info_t hash_pkg_info;
            if (upkg_hash_convert_package_info(&pkg_info, &hash_pkg_info) == 0) {
                if (upkg_hash_add_package(upkg_main_hash_table, &hash_pkg_info) == 0) {
                    printf("Package successfully added to internal database.\n\n");
                    
                    // Test: Search and print from hash table to verify integrity
                    upkg_hash_package_info_t *stored_pkg = upkg_hash_search(upkg_main_hash_table, pkg_info.package_name);
                    if (stored_pkg) {
                        upkg_hash_print_package_info(stored_pkg);
                    } else {
                        printf("Warning: Package not found in hash table after adding.\n");
                    }
                } else {
                    printf("Warning: Failed to add package to internal database.\n");
                }
                // Note: hash_pkg_info memory is managed by the hash table now
            } else {
                printf("Warning: Failed to convert package info for hash table.\n");
            }
        }
        
        if (g_verbose_mode && g_system_install_root) {
            printf("Installation Configuration:\n");
            printf("=========================\n");
            printf("  Control dir: %s\n", g_control_dir);
            printf("  Install root: %s\n", g_system_install_root);
            printf("\n");
        }
        
        printf("Package information collection completed successfully.\n");
        printf("Note: Actual installation logic is not yet implemented.\n");
    } else {
        printf("Error: Failed to extract package or collect information.\n");
    }
    
    // Clean up allocated memory
    upkg_pack_free_package_info(&pkg_info);
}

/**
 * @brief Handles package removal (placeholder).
 */
void handle_remove(const char *package_name) {
    printf("Removing package: %s (placeholder)\n", package_name);
}

/**
 * @brief Lists installed packages (placeholder).
 */
void handle_list(void) {
    upkg_log_verbose("Listing installed packages...\n");
    printf("Listing installed packages... (placeholder)\n");
    if (g_db_dir) {
        upkg_log_verbose("  Database dir: %s\n", g_db_dir);
    }
}

/**
 * @brief Shows package status (placeholder).
 */
void handle_status(const char *package_name) {
    printf("Showing status for package: %s (placeholder)\n", package_name);
}

/**
 * @brief Searches for packages (placeholder).
 */
void handle_search(const char *query) {
    printf("Searching for packages with query: %s (placeholder)\n", query);
}

/**
 * @brief Prints the current configuration values.
 */
void handle_print_config(void) {
    printf("upkg Configuration:\n");
    printf("==================\n");
    if (g_upkg_base_dir) {
        printf("  Base Directory:     %s\n", g_upkg_base_dir);
    } else {
        printf("  Base Directory:     (not set)\n");
    }
    if (g_control_dir) {
        printf("  Control Directory:  %s\n", g_control_dir);
    } else {
        printf("  Control Directory:  (not set)\n");
    }
    if (g_install_dir_internal) {
        printf("  Install Directory:  %s\n", g_install_dir_internal);
    } else {
        printf("  Install Directory:  (not set)\n");
    }
    if (g_system_install_root) {
        printf("  System Install Root: %s\n", g_system_install_root);
    } else {
        printf("  System Install Root: (not set)\n");
    }
     if (g_db_dir) {
        printf("  Database Directory: %s\n", g_db_dir);
    } else {
        printf("  Database Directory: (not set)\n");
    }
}

/**
 * @brief Prints the path to the configuration file in use.
 */
void handle_print_config_file(void) {
    char *config_path = upkg_get_config_file_path();
    if (config_path) {
        printf("Configuration file in use: %s\n", config_path);
        free(config_path);
    } else {
        printf("No configuration file found.\n");
        printf("Searched locations:\n");
        printf("  1. $UPKG_CONFIG_PATH environment variable\n");
        printf("  2. /etc/upkg/upkgconfig (system-wide)\n");
        printf("  3. ~/.upkgconfig (user-specific)\n");
    }
}

// --- Main Function ---
int main(int argc, char *argv[]) {
    // Check for verbose mode first, as it affects all subsequent output.
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            g_verbose_mode = true;
            break;
        }
    }

    // Handle help or version before initialization
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            usage();
            return EXIT_SUCCESS;
        }
        if (strcmp(argv[i], "--version") == 0) {
            handle_version();
            return EXIT_SUCCESS;
        }
        if (strcmp(argv[i], "--print-config") == 0) {
            // Initialize first to load config, then print
            if (upkg_init() != 0) {
                fprintf(stderr, "ERROR: Failed to initialize upkg configuration\n");
                return EXIT_FAILURE;
            }
            handle_print_config();
            upkg_cleanup();
            return EXIT_SUCCESS;
        }
        if (strcmp(argv[i], "--print-config-file") == 0) {
            // No need to initialize for this - just find the config file
            handle_print_config_file();
            return EXIT_SUCCESS;
        }
    }

    if (argc < 2) {
        usage();
        return EXIT_FAILURE;
    }
    
    // --- Core Program Flow ---
    // Step 1: Initialize the environment and load the database
    upkg_log_verbose("Starting upkg with %d arguments\n", argc);
    if (upkg_init() != 0) {
        errormsg("Critical error during program initialization. Exiting.\n");
        upkg_cleanup();
        return EXIT_FAILURE;
    }
    
    // Register the cleanup function to be called on exit.
    atexit(upkg_cleanup);

    // Step 2: Execute commands based on the interleaved arguments.
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--install") == 0) {
            if (i + 1 < argc) {
                // Loop to handle multiple .deb files
                while (i + 1 < argc) {
                    char *next_arg = argv[i+1];
                    if (next_arg[0] == '-' || strstr(next_arg, ".deb") == NULL) {
                        break; // Stop if it's a new command switch or not a .deb file
                    }
                    handle_install(next_arg);
                    i++;
                }
            } else {
                errormsg("Error: -i/--install requires at least one .deb file argument.");
            }
        } else if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--remove") == 0) {
            if (i + 1 < argc) {
                handle_remove(argv[i+1]);
                i++;
            } else {
                errormsg("Error: -r/--remove requires a package name.");
            }
        } else if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--list") == 0) {
            handle_list();
        } else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--status") == 0) {
            if (i + 1 < argc) {
                handle_status(argv[i+1]);
                i++;
            } else {
                errormsg("Error: -s/--status requires a package name.");
            }
        } else if (strcmp(argv[i], "-S") == 0 || strcmp(argv[i], "--search") == 0) {
            if (i + 1 < argc) {
                handle_search(argv[i+1]);
                i++;
            } else {
                errormsg("Error: -S/--search requires a query.");
            }
        } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            // Already handled at the start of main
        } else {
            errormsg("Error: Unknown argument or command: %s", argv[i]);
            // For interleaved commands, we should continue processing if possible
            // For now, let's just break on an unknown command
            break;
        }
    }

    return EXIT_SUCCESS;
    // Note: The atexit handler will now call upkg_cleanup()
}
