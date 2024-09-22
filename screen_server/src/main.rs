use minifb::{Key, Scale, WindowOptions};
use screen_server::{
    map::BitMap,
    tcp::{self, Listener, TcpCmdHandler, TcpError},
    Screen, ScreenError,
};
use std::{sync::mpsc, thread, u32};

// TcpCmdHandler 和 Listener 定义略

const WIDTH: usize = 128;
const HEIGHT: usize = 64;

struct TestCmd {
    id: usize,
}

impl TcpCmdHandler for TestCmd {
    fn response(&self, cmd: char) -> String {
        "Hi".to_string()
    }

    fn accept(&self, buf: Vec<u8>) -> Result<(), tcp::TcpError> {
        println!(
            "print: ({}, {})",
            (buf[1] as u16) << 8 | buf[2] as u16,
            (buf[3] as u16) << 8 | buf[4] as u16
        );
        Ok(())
    }
}
struct Protocol {
    command: u8,
    pos_x: u16,
    pos_y: u16,
}
impl Protocol {
    pub fn from_tcp_buffer(buffer: Vec<u8>) -> Self {
        let pos_x: u16 = (buffer[1] as u16) << 8 | buffer[2] as u16;
        let pos_y: u16 = (buffer[3] as u16) << 8 | buffer[4] as u16;
        Protocol {
            command: buffer[0],
            pos_x,
            pos_y,
        }
    }
}
fn main() -> Result<(), ScreenError> {
    let cmd = TestCmd { id: 0 };
    let listener = Listener::new("127.0.0.1:8080", cmd, 5)?;

    let mut bmap = BitMap::new(
        "new_window",
        WIDTH,
        HEIGHT,
        WindowOptions {
            scale: Scale::X2,
            ..WindowOptions::default()
        },
    )?;

    let mut screen_ser = Screen::new(listener, bmap)?;

    let (tx, rx) = mpsc::channel();
    let listener = screen_ser.listener;
    screen_ser.map.window.set_target_fps(120);
    let mut window = screen_ser.map.window;

    let mut buffer = vec![0; WIDTH * HEIGHT];
    let mut new_buffer = vec![0; WIDTH * HEIGHT];

    let tcp_tx = tx.clone();
    thread::spawn(move || {
        if let Ok(mut stream) = listener.tcp_listener.incoming().next().unwrap() {
            loop {
                match listener.get_data(&mut stream) {
                    Ok(buff) => {
                        let _ = listener.tcp_cmd.accept(buff.clone());
                        if tcp_tx.send(buff).is_err() {
                            println!("Failed to send data to main thread.");
                            break;
                        }
                    }
                    Err(TcpError::Accept(_)) => {
                        println!("Client disconnected.");
                        break;
                    }
                    Err(e) => {
                        eprintln!("Error occurred: {:?}", e);
                        break;
                    }
                }
            }
        }
    });

    // 初始化显示缓冲区
    for i in buffer.iter_mut() {
        *i = u32::MAX;
    }
    for i in new_buffer.iter_mut() {
        *i = u32::MAX;
    }
    /*
    Tcp Buffer structure {
        command
        high byte of position X
        low byte of position X
        high byte of position Y
        low byte of position Y

    }
    */
    let mut update_flag = false;
    while window.is_open() && !window.is_key_down(Key::Escape) {
        if let Ok(dta) = rx.try_recv() {
            if dta.len() != 0 {
                let protocol = Protocol::from_tcp_buffer(dta);
                /*
                deal with command
                */
                match protocol.command {
                    0 => {
                        let pos = ((protocol.pos_y * (WIDTH as u16)) + protocol.pos_x) as usize;
                        if pos < new_buffer.len() {
                            new_buffer[pos] = 0;
                            update_flag = true;
                        }
                    }
                    1 => {
                        let pos = ((protocol.pos_y * (WIDTH as u16)) + protocol.pos_x) as usize;
                        if pos < new_buffer.len() {
                            new_buffer[pos] = 0;
                        }
                    }
                    2 => {
                        update_flag = true;
                    }
                    3 => {
                        println!("buffer cleanup");
                        new_buffer.fill(u32::MAX);
                    }
                    _ => {}
                }
            }
        }
        if update_flag {
            window.update_with_buffer(&new_buffer, WIDTH, HEIGHT)?;
            buffer = new_buffer.clone();
            update_flag = false;
        } else {
            window.update_with_buffer(&buffer, WIDTH, HEIGHT)?;
        }
    }

    Ok(())
}
