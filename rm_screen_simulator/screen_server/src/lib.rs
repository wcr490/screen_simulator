pub mod map;
pub mod tcp;

use std::sync::mpsc;

use map::BitMap;
use tcp::Listener;
use thiserror::Error;

#[derive(Error, Debug)]
pub enum ScreenError {
    #[error("Failed to deal with this message")]
    Accept(String),

    #[error("Io error occurred {0}")]
    Io(#[from] std::io::Error),

    #[error("Failed to create window occurred {0}")]
    WindowCreate(#[from] minifb::Error),

    #[error("Failed with bitmap occurred {0}")]
    BitMap(#[from] map::MapError),

    #[error("Failed with tcp occurred {0}")]
    Tcp(#[from] tcp::TcpError),

    #[error("The Channel is hung up")]
    Recv(#[from] mpsc::RecvError),
}
pub struct Screen<T: tcp::TcpCmdHandler> {
    pub listener: tcp::Listener<T>,
    pub map: map::BitMap,
}

impl<T: tcp::TcpCmdHandler> Screen<T> {
    pub fn new(listener: Listener<T>, map: BitMap) -> Result<Self, ScreenError> {
        Ok(Screen { listener, map })
    }
}
