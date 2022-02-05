//
// Created by janusz on 03.02.2022.
//

#include "tcp_cho_server.h"
#include "queue_array.h"
#include "circular_buffer.h"

int main(){

    //start_server();
    queue q;

    init_queue(&q,7);

    add_queue(&q,2);
    add_queue(&q,3);
    add_queue(&q,4);
    add_queue(&q,5);
    add_queue(&q,6);
    add_queue(&q,7);
    add_queue(&q,8);
    add_queue(&q,9);

    delete_from_queue(&q);

    add_queue(&q,10);

    delete_from_queue(&q);

    add_queue(&q,11);

    print_queue(&q);

    queue_destroy(&q);

    std::cout << "-------------------------------\n";

    circular_buffer<int> buffer(5);
    buffer.add_value(1);
    buffer.add_value(2);
    buffer.add_value(3);
    buffer.add_value(4);
    buffer.add_value(5);

    buffer.print_buffer();

    auto v = buffer.delete_value();

    std::cout << "\nDeleted value: " << v.value() << "\n";

    buffer.add_value(6);

    buffer.print_buffer();

    v = buffer.delete_value();

    std::cout << "\nDeleted value: " << v.value() << "\n";

    buffer.add_value(7);

    buffer.print_buffer();


    return 0;
}
