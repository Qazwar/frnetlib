//
// Created by fred on 06/12/16.
//

#ifndef FRNETLIB_SOCKET_H
#define FRNETLIB_SOCKET_H


#include "NetworkEncoding.h"
#include "Packet.h"

namespace fr
{
    class Socket
    {
    public:
        enum Status
        {
            Unknown = 0,
            Success = 1,
            ListenFailed = 2,
            BindFailed = 3,
            Disconnected = 4,
            Error = 5,
            WouldBlock = 6,
            ConnectionFailed = 7,
            HandshakeFailed = 8,
            VerificationFailed = 9,
        };

        Socket() noexcept;
        virtual ~Socket() noexcept = default;

        /*!
         * Close the connection.
         */
        virtual void close()=0;

        /*!
         * Connects the socket to an address.
         *
         * @param address The address of the socket to connect to
         * @param port The port of the socket to connect to
         * @return A Socket::Status indicating the status of the operation.
         */
        virtual Socket::Status connect(const std::string &address, const std::string &port)=0;

        /*!
         * Gets the socket's printable remote address
         *
         * @return The string address
         */
        inline const std::string &get_remote_address()
        {
            return remote_address;
        }

        /*!
         * Sets the socket to blocking or non-blocking.
         *
         * @param should_block True for blocking (default argument), false otherwise.
         */
        virtual void set_blocking(bool should_block = true) = 0;

        /*!
         * Attempts to send raw data down the socket, without
         * any of frnetlib's framing. Useful for communicating through
         * different protocols.
         *
         * @param data The data to send.
         * @param size The number of bytes, from data to send. Be careful not to overflow.
         * @return The status of the operation.
         */
        virtual Status send_raw(const char *data, size_t size) = 0;


        /*!
         * Receives raw data from the socket, without any of
         * frnetlib's framing. Useful for communicating through
         * different protocols. This will attempt to read 'data_size'
         * bytes, but might not succeed. It'll return how many bytes were actually
         * read in 'received'.
         *
         * @param data Where to store the received data.
         * @param data_size The number of bytes to try and receive. Be sure that it's not larger than data.
         * @param received Will be filled with the number of bytes actually received, might be less than you requested.
         * @return The status of the operation, if the socket has disconnected etc.
         */
        virtual Status receive_raw(void *data, size_t data_size, size_t &received) = 0;

        /*!
         * Send a packet through the socket
         *
         * @param packet The packet to send
         * @return True on success, false on failure.
         */
        Status send(const Packet &packet);

        /*!
         * Receive a packet through the socket
         *
         * @param packet The packet to receive
         * @return True on success, false on failure.
         */
        Status receive(Packet &packet);

        /*!
         * Reads size bytes into dest from the socket.
         * Unlike receive_raw, this will keep trying
         * to receive data until 'size' bytes have been
         * read, or the client has disconnected/there was
         * an error.
         *
         * @param dest Where to read the data into
         * @param size The number of bytes to read
         * @return Operation status.
         */
        Status receive_all(void *dest, size_t size);

        /*!
         * Checks to see if we're connected to a socket or not
         *
         * @return True if it's connected. False otherwise.
         */
        inline bool connected() const
        {
            return is_connected;
        }

        /*!
         * Gets the socket descriptor.
         *
         * @return The socket descriptor.
         */
        virtual int32_t get_socket_descriptor() const = 0;

    protected:
        std::string remote_address;
        bool is_blocking;
        bool is_connected;
    };
}


#endif //FRNETLIB_SOCKET_H
