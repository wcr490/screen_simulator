use std::{
    io::{Read, Write},
    net::{TcpListener, TcpStream, ToSocketAddrs},
};

use thiserror::Error;

#[derive(Error, Debug)]
pub enum TcpError {
    #[error("Failed to deal with this message")]
    Accept(String),

    #[error("Io error occurred {0}")]
    Io(#[from] std::io::Error),
}

pub struct Listener<T: TcpCmdHandler> {
    pub tcp_listener: TcpListener,
    pub tcp_cmd: T,
    pub max_buffer_size: usize,
}

pub trait TcpCmdHandler {
    fn response(&self, cmd: char) -> String;
    fn accept(&self, buf: Vec<u8>) -> Result<(), TcpError>;
}

impl<T: TcpCmdHandler> Listener<T> {
    pub fn new<A: ToSocketAddrs>(
        addr: A,
        cmd: T,
        max_buffer_size: usize,
    ) -> Result<Self, TcpError> {
        Ok(Listener {
            tcp_listener: TcpListener::bind(addr)?,
            tcp_cmd: cmd,
            max_buffer_size,
        })
    }
    pub fn get_data(&self, stream: &mut TcpStream) -> Result<Vec<u8>, TcpError> {
        let mut buf = vec![0; self.max_buffer_size];

        let bytes_len = stream.read(&mut buf)?;
        if bytes_len == 0 {
            return Err(TcpError::Accept("Finished".to_string()));
        }
        Ok(buf)
    }
}
