// https://rust-unofficial.github.io/too-many-lists/index.html

pub struct List<T> {
  head: Link<T>,
}

type Link<T> = Option<Box<Node<T>>>;

struct Node<T> {
  elem: T,
  next: Link<T>,
}

impl<T> List<T> {
  pub fn new() -> Self {
      List { head: None }
  }

  pub fn push(&mut self, elem: T) {
      let new_node = Box::new(Node {
          elem: elem,
          next: self.head.take(),
      });

      self.head = Some(new_node);
  }

  pub fn pop(&mut self) -> Option<T> {
      self.head.take().map(|node| {
          self.head = node.next;
          node.elem
      })
  }

  pub fn peek(&self) -> Option<&T> {
      self.head.as_ref().map(|node| {
          &node.elem
      })
  }

  pub fn peek_mut(&mut self) -> Option<&mut T> {
      self.head.as_mut().map(|node| {
          &mut node.elem
      })
  }

  pub fn into_iter(self) -> IntoIter<T> {
      IntoIter(self)
  }

  pub fn iter(&self) -> Iter<'_, T> {
      Iter { next: self.head.as_ref().map(|node| &**node) }
  }

  pub fn iter_mut(&mut self) -> IterMut<'_, T> {
      IterMut { next: self.head.as_mut().map(|node| &mut **node) }
  }
}

impl<T> Drop for List<T> {
  fn drop(&mut self) {
      let mut cur_link = self.head.take();
      while let Some(mut boxed_node) = cur_link {
          cur_link = boxed_node.next.take();
      }
  }
}

pub struct IntoIter<T>(List<T>);

impl<T> Iterator for IntoIter<T> {
  type Item = T;
  fn next(&mut self) -> Option<Self::Item> {
      // access fields of a tuple struct numerically
      self.0.pop()
  }
}

pub struct Iter<'a, T> {
  next: Option<&'a Node<T>>,
}

impl<'a, T> Iterator for Iter<'a, T> {
  type Item = &'a T;
  fn next(&mut self) -> Option<Self::Item> {
      self.next.map(|node| {
          self.next = node.next.as_ref().map(|node| &**node);
          &node.elem
      })
  }
}

pub struct IterMut<'a, T> {
  next: Option<&'a mut Node<T>>,
}

impl<'a, T> Iterator for IterMut<'a, T> {
  type Item = &'a mut T;

  fn next(&mut self) -> Option<Self::Item> {
      self.next.take().map(|node| {
          self.next = node.next.as_mut().map(|node| &mut **node);
          &mut node.elem
      })
  }
}

fn main() {

  let mut list = List::new();

  list.push(1);
  list.push(2);
  list.push(3);

  println!("{:?}", list.peek())

}
