# Tattoo and Piercing Shop Simulation

This C program simulates the operation of a tattoo and piercing shop. It uses pthreads for multithreading and semaphores to manage access to shared resources. The simulation includes the following features:

- Random arrival of clients for either tattoo or body piercing services.
- Clients can either be served immediately or wait in a designated area if the tattoo or piercing artist is busy.
- The number of clients served, the number of clients who had to leave due to unavailability, and other statistics are tracked and displayed at the end of the simulation.

## Usage

To run the simulation, use the following command:

```bash
./tattooshop [Number of Clients] [Number of Tattoo Puffs] [Number of Piercing Puffs] [Probability of Tattoo] [Min Service Time] [Max Service Time]
```

- `Number of Clients`: The total number of clients that will visit the shop during the simulation.
- `Number of Tattoo Puffs`: The number of available tattoo stations.
- `Number of Piercing Puffs`: The number of available piercing stations.
- `Probability of Tattoo`: The probability that a client will request a tattoo (a float value between 0 and 0.99).
- `Min Service Time`: The minimum time a client will be served by a professional (in milliseconds).
- `Max Service Time`: The maximum time a client will be served by a professional (in milliseconds).

## Description

This simulation program creates three types of threads: clients, a tattoo artist, and a body piercer. The clients arrive randomly and can request either a tattoo or a body piercing. They are assigned to available professionals based on the service they require. If a professional is available, the client is served immediately; otherwise, they wait in designated areas for either service.

The simulation continues until the number of clients served reaches the maximum allowed, and the results are displayed at the end.

## Error Handling

The program performs error checks on the input parameters to ensure they are within reasonable ranges. If the inputs are incorrect, it provides a usage message and exits with an error code.

## Synchronization

The program uses semaphores to synchronize access to shared resources, such as the available puff chairs and the professionals. This ensures that multiple threads do not access the same resource simultaneously and allows for a realistic simulation of client flow.

## Terminating the Simulation

The simulation ends when the maximum number of clients has been served, and all threads have completed their tasks. The total number of clients served, clients served by each professional, clients who left without being served, and clients served without waiting are displayed at the end of the simulation.

## Note

This code was created for the UPF Operating Systems 1 discipline.
