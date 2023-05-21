use std::net::{TcpListener, TcpStream};
use std::io::{Read, Write};
use std::thread;

fn handle_client(mut stream: TcpStream) {
    let mut buffer = [0; 512];
    loop {
        let bytes_read = stream.read(&mut buffer).expect("Failed to read from client");
        if bytes_read == 0 { return; } // client closed connection
        stream.write(&buffer[0..bytes_read]).expect("Failed to write to client");
    }
}

fn main() {
    let listener = TcpListener::bind("0.0.0.0:8080").expect("Could not bind");
    for stream in listener.incoming() {
        match stream {
            Ok(stream) => {
                thread::spawn(|| { // spawn new thread to handle client
                    handle_client(stream);
                });
            }
            Err(e) => { eprintln!("Failed to accept connection: {}", e); }
        }
    }
}

