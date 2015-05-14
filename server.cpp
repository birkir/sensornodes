#include <ctime>
#include <iostream>
#include <pqxx/pqxx>
#include <RF24/RF24.h>
#include <RF24Network/RF24Network.h>

RF24 radio(25, RPI_BPLUS_GPIO_J8_24, BCM2835_SPI_SPEED_8MHZ);
RF24Network network(radio);

enum payloadTypes {
  payloadCommand,
  payloadReadingChar,
  payloadReadingInteger,
  payloadReadingFloat,
  payloadReadingDouble
};

struct command { char message[32]; };
struct readingInt { char name[16]; int value; };
struct readingFloat { char name[16]; float value; };
struct readingChar { char name[16]; char value[16]; };
struct readingDouble { char name[16]; double value; };

int main(int argc, char* argv[])
{

	// Setup connection to Postgres SQL
	pqxx::connection conn("dbname=sensornodes user=postgres password=postgres");
	pqxx::work db(conn);

  // Setup radio
  radio.begin();
  delay(5);

  // Setup network on channel 90 with our id as zero (0)
  network.begin(90, 00);
  radio.printDetails();

  printf("starting server...\n");

  while (true)
  {
    network.update();

    if (false) {
      command cmd;
      strncpy(cmd.message, "SET INTERVAL=10", sizeof(cmd.message));
      RF24NetworkHeader h(02, payloadCommand);
      network.write(h, &cmd, sizeof(cmd));
    }

    while (network.available())
    {
      RF24NetworkHeader header;
      network.peek(header);

      // Generate time for better display
      time_t rawtime;
      struct tm * timeinfo;
      char ts[32];
      time (&rawtime);
      timeinfo = localtime(&rawtime);
      strftime(ts, 32, "[%X]", timeinfo);

      // And some coloring
      char info[64];
      sprintf(info, "%s [\x1b[33mnode %i\x1b[0m]", ts, header.from_node);

      if (header.type == payloadReadingFloat) {
        readingFloat reading;
        network.read(header, &reading, sizeof(reading));
        printf("%s %s\t\x1b[37m%f\x1b[0m\n", info, reading.name, reading.value);
      }

      if (header.type == payloadReadingInteger) {
        readingInt reading;
        network.read(header, &reading, sizeof(reading));
        printf("%s %s\t%i\n", info, reading.name, reading.value);
      }

      if (header.type == payloadReadingChar) {
        readingChar reading;
        network.read(header, &reading, sizeof(reading));
        printf("%s %s: %s\n", info, reading.name, reading.value);
      }
    }
  }

	return 0;
}
