#include <iostream>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/timerfd.h>
#include <sys/unistd.h>
#include <mosquitto.h>

#include "generators.hpp"
#include "netutils.hpp"

uint8_t send_buf[BUF_LEN];

bool rand_vals::air_moisture_flag = true;
bool rand_vals::soil_moisture_flag = true;
uint8_t rand_vals::last_air_moisture;
uint8_t rand_vals::last_soil_moisture;

int main (int argc, char **argv)
{
    mosquitto *mqtt;
    mosquitto_lib_init();
    mqtt = mosquitto_new(argv[1], true, NULL);
    int rc = mosquitto_connect(mqtt, argv[2], 1883, 60);
	if(rc != 0){
		printf("Client could not connect to broker! Error Code: %d\n", rc);
		mosquitto_destroy(mqtt);
		return 1;
	}

    size_t garden_id = atoi(argv[1]);
    rand_vals::init(static_cast<size_t>(atoi(argv[1])));
    auto gen = generators::garden_info_generator();

    int _timerfd = timerfd_create(CLOCK_REALTIME, 0);
    if ( _timerfd < 0 ) {
        fprintf(stderr, "timerfd_create() failed with %s\n", strerror(errno));
        return 1;
    }

    int _epollfd = epoll_create1(0);
    if ( _epollfd < 0 ) {
        fprintf(stderr, "epoll_create1() failed with %s\n", strerror(errno));
        close(_timerfd);
        return 1;
    }

    epoll_event ev, revents[MAX_EPOLL_REVENTS];
    ev.events = EPOLLIN;
    ev.data.fd = _timerfd;
    if ( epoll_ctl(_epollfd, EPOLL_CTL_ADD, _timerfd, &ev) == -1 ) {
        fprintf(stderr, "epoll_ctl_add() failed with %s\n", strerror(errno));
        close(_timerfd);
        close(_epollfd);
        return 1;
    }

    timespec now;
    itimerspec new_value;

    if ( clock_gettime(CLOCK_REALTIME, &now) == -1 ) {
        fprintf(stderr, "clock_get_time() failed with %s\n", strerror(errno));
        close(_timerfd);
        close(_epollfd);
        return 1;
    }

    new_value.it_value.tv_sec = now.tv_sec + 5;
    new_value.it_value.tv_nsec = 0;
    new_value.it_interval.tv_sec = 5;
    new_value.it_interval.tv_nsec = 0;

    timerfd_settime(_timerfd, TFD_TIMER_ABSTIME, &new_value, NULL);

    int send_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if ( send_socket < 0 ) {
        fprintf(stderr, "socket() failed with %s\n", strerror(errno));
        close(_timerfd);
        close(_epollfd);
        return 1;
    }

    sockaddr_in master_addr;
    rc = netutils::ipv4_addr(argv[2], 12345, master_addr);
    if ( rc < 0 ) {
        fprintf(stderr, "netutils::ipv4_addr() failed with %s\n", strerror(errno));
        close(_timerfd);
        close(_epollfd);
        close(send_socket);
        return 1;
    }

    for ( ;; ) {
        int ndfs = epoll_wait(_epollfd, revents, MAX_EPOLL_REVENTS, -1);

        if ( ndfs == -1 ) {
            fprintf(stderr, "epoll_wait() failed with %s\n", strerror(errno));
            break;
        }

        read(revents[0].data.fd, send_buf, BUF_LEN);
        if ( gen ) {
            metrics::garden_info _info = gen();
            _info._garden_id = garden_id;
            std::string const to_send = _info.convert_to(0, 1);
            std::cout << to_send << std::endl;
            // rc = sendto(send_socket, to_send.c_str(), to_send.size(), 0, (sockaddr *)&master_addr,
            //        sizeof(master_addr));
            rc = mosquitto_publish(mqtt, NULL, "metrics", to_send.size(), to_send.c_str(), 0, false);
            if ( rc != MOSQ_ERR_SUCCESS ) {
                fprintf(stderr, "mosquitto_publish() failed with %s\n", strerror(errno));
                // break;
            }
        } else {
            fprintf(stderr, "generator can not create values anymore\n");
            break;
        }
    }

    close(_timerfd);
    close(_epollfd);
    close(send_socket);
    mosquitto_disconnect(mqtt);
	mosquitto_destroy(mqtt);

	mosquitto_lib_cleanup();
    return 0;
}