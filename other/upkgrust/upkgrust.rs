use std::env;

fn main() {
    if env::args().count() < 2 {
        println!("Usage: Please provide at least one argument.");
        return;
    }
    let args: Vec<String> = env::args().collect(); 
    for arg in args.iter().skip(1) { // Skip the program name
        if arg == "-v" || arg == "--version" {
            println!("upkgrust (ulinux) 1.0");
        } else if arg == "-h" || arg == "--help" {
            println!("usage option and help message...");
        } else {
            println!("Unknown option: {}", arg);
        }
    }
}
