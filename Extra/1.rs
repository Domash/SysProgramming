use std::ffi::OsStr;

use std::io;
use std::io::Write;
use std::io::BufRead;

use std::fs;
use std::fs::File;
use std::fs::OpenOptions;

const TARGET_FILE: &str = "./result.txt";

pub fn path_exists(path: &str) -> bool {
  fs::metadata(path).is_ok()
}

pub fn trim_newline(line: &mut String) {
  if line.ends_with('\n') {
    line.pop();
    if line.ends_with('\r') {
      line.pop();
    }
  }
}

fn main() {

  let mut extension_pattern = String::new();

  io::stdin().lock().read_line(&mut extension_pattern).expect("Error, Bad line!");
  trim_newline(&mut extension_pattern);

  let extension_pattern_os_str = OsStr::new(&extension_pattern);

  if !path_exists(TARGET_FILE) {
    File::create(TARGET_FILE).expect("Error, Unable to create file!");
  }

  let mut file = OpenOptions::new().write(true)
                                   .append(true)
                                   .open(TARGET_FILE)
                                   .expect("Error, Unable to open file!");

  for dir_entry in fs::read_dir("./").unwrap() {

    let dir = dir_entry.unwrap();
    let path = dir.path();
    let extension = path.extension();

    if extension != None {

      let extension_os_str = extension.unwrap().to_os_string();
      let path_str: &str = path.file_name().unwrap().to_str().unwrap();

      if extension_os_str == extension_pattern_os_str {
        writeln!(&mut file, "{}", path_str).expect("Error, unable write to file"); 
      }

    }

  }

}