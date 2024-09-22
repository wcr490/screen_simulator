use minifb::{Window, WindowOptions};
use thiserror::Error;

/* Carefully: The size of window should be fixed or exit */

#[derive(Error, Debug)]
pub enum MapError {
    #[error("Failed to create window occurred {0}")]
    WindowCreate(#[from] minifb::Error),
}
pub struct BitMap {
    pub buffer: Vec<u32>,
    pub window: Window,
}

impl BitMap {
    pub fn new(
        name: &str,
        width: usize,
        height: usize,
        opts: WindowOptions,
    ) -> Result<Self, MapError> {
        let window = Window::new(name, width, height, opts)?;
        Ok(BitMap {
            buffer: vec![0u32; width * height],
            window,
        })
    }
    pub fn update_all(&mut self, new_buffer: Vec<u32>) -> Result<(), MapError> {
        /* It will cause problems if the size is altered */
        self.buffer = new_buffer;
        self.window.update_with_buffer(
            &self.buffer,
            self.window.get_size().0,
            self.window.get_size().1,
        )?;
        Ok(())
    }
}
