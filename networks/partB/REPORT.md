## Introduction

In this project, we implemented certain aspects of TCP functionality using UDP sockets. Specifically, we focused on data sequencing and retransmission. This report discusses the details of our implementation and how it differs from traditional TCP. Additionally, we explore the possibility of extending our implementation to account for flow control.

## How is your implementation of data sequencing and retransmission different from traditional TCP? (If there are no apparent differences, you may mention that) (3 marks)

Here are the key differences:

Connectionless vs. Connection-Oriented: UDP is a connectionless protocol, while TCP is connection-oriented. In my implementation, there is no initial handshake or connection setup like in TCP. The sender can start sending data without establishing a connection, and the receiver listens for incoming packets without the need for a connection state.

Reliability Mechanisms: In TCP, reliability mechanisms such as sequence numbers, acknowledgments, and window sizes are built into the protocol itself. TCP ensures the ordered and reliable delivery of data through these mechanisms. In my UDP-based implementation, I have had to implement these mechanisms manually. I assign sequence numbers to chunks of data and implement retransmissions in response to missing acknowledgments to achieve reliability.

Flow Control: TCP has built-in flow control mechanisms like the sliding window, which prevents the sender from overwhelming the receiver with data. In my implementation, there is no inherent flow control, and i've not implemented such a mechanism. The sender continues to transmit chunks without considering the receiver's capacity.

In summary, my implementation using UDP with data sequencing and retransmission is different from traditional TCP in terms of connection handling, reliability mechanisms, flow control, congestion control, guaranteed delivery, and overhead. While TCP provides a higher level of reliability and robustness for data transmission, my UDP-based implementation gives you more flexibility but requires manual management of these aspects.

## How can you extend your implementation to account for flow control? You may ignore deadlocks. (5 marks)

To extend my UDP-based implementation to account for flow control, you can implement a simple flow control mechanism based on a sliding window. Flow control will help prevent the sender from overwhelming the receiver with data, ensuring efficient data transmission. Here are the steps to add flow control to my implementation:

Define a Sliding Window: Create a sliding window at both the sender and receiver sides. The window size determines the maximum number of unacknowledged packets the sender can have in-flight.

Sender Side:

Modify the sender to maintain a sending window. This window keeps track of which packets can be sent.
Initially, fill the sending window with packets (chunks) up to the window size and start sending them.
As acknowledgments are received, slide the window forward and send new packets to replace the acknowledged ones.
Receiver Side:

Implement a receiving window that keeps track of the expected sequence numbers.
When a packet arrives, check if it falls within the receiving window. If it does, process it; otherwise, discard it as it's out of order.
Send acknowledgments for the correctly received packets.
Sender Flow Control:

Before sending a new packet, check if there is space available in the sending window.
If the window is full, the sender should wait until acknowledgments are received, freeing up space in the window.
Adjust the window size based on the receiver's capacity and network conditions. You can dynamically increase or decrease the window size.
Receiver Flow Control:

Implement a mechanism to inform the sender about its current buffer or processing capacity. You can send this information as part of ACKs or in separate messages.
The sender should periodically check this information and adjust its transmission rate accordingly.
Timeouts and Retransmissions:

When implementing retransmissions, ensure that you only retransmit packets that are within the sending window and haven't been acknowledged yet.
Error Handling:

Handle cases where the sender's sending window exceeds the receiver's receiving window capacity. This situation might indicate a potential bottleneck or congestion in the network.
By implementing these flow control mechanisms, my UDP-based data transmission system will become more efficient and adaptive to the receiver's capacity. It will help prevent data loss due to receiver buffer overflow and improve the overall reliability of my application.

## ADITYA RAGHUVANSHI
