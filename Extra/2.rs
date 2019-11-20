use std::io;
use std::io::BufRead;

use std::fs;

use std::path::Path;

use std::cmp::Ordering;

const SIZE: u64 = 1000 * 1000 * 100;

pub fn trim_newline(line: &mut String) {
  if line.ends_with('\n') {
    line.pop();
    if line.ends_with('\r') {
      line.pop();
    }
  }
}

pub fn dfs(dir: &Path, vec: &mut Vec<Info>, size: u64) -> io::Result<()> {
  if dir.is_dir() {
    for entry in fs::read_dir(dir)? {
      let entry = entry?;
      let path  = entry.path();
      if path.is_dir() {
        dfs(&path, vec, size)?;
      } else {
        let metadata = fs::metadata(path.clone())?;
        if metadata.len() > size {
          vec.push(Info{size: metadata.len(), path: path.clone().into_os_string().into_string().unwrap()})
        }
      }
    }
  }
  Ok(())
}

#[derive(Eq)]
pub struct Info {
  size: u64,
  path: String,
}

impl Ord for Info {
  fn cmp(&self, other: &Self) -> Ordering {
    (other.size, &other.path).cmp(&(self.size, &self.path))
  }
}

impl PartialOrd for Info {
  fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
    Some(self.cmp(other))
  }
}

impl PartialEq for Info {
  fn eq(&self, other: &Self) -> bool {
    (self.size, &self.path) == (other.size, &other.path)
  }
}

fn main() {  

  // /Users/denisdomasevich/Downloads/
  let mut root_dir = String::new();

  io::stdin().lock().read_line(&mut root_dir).expect("Error, bad line!");
  trim_newline(&mut root_dir);

  let mut vec: Vec<Info> = Vec::new();

  let _result = dfs(Path::new(&root_dir), &mut vec, SIZE);

  for info in vec {
    println!("Path = {}, size = {} bytes", info.path, info.size);
  }

}