#include <stdio.h>
#include <stdlib.h>
#include <dbus/dbus.h>

int main(int argc, char** argv) {

    // Định nghĩa các biến và thông tin về module
    DBusConnection* connection;
    DBusError error;
    DBusMessage* message;
    DBusMessageIter args;
    DBusPendingCall* pending;
    int ret;
    char* param = "parameter";

    // Khởi tạo kết nối với DBus
    dbus_error_init(&error);
    connection = dbus_bus_get(DBUS_BUS_SESSION, &error);

    if (dbus_error_is_set(&error)) {
        fprintf(stderr, "Error connecting to the D-Bus daemon: %s\n", error.message);
        dbus_error_free(&error);
        return EXIT_FAILURE;
    }

    // Gửi yêu cầu đến module
    message = dbus_message_new_method_call("com.example.MyModule", "/com/example/MyModule",
                                            "com.example.MyInterface", "MyMethod");

    if (message == NULL) {
        fprintf(stderr, "Error creating D-Bus message\n");
        return EXIT_FAILURE;
    }

    dbus_message_iter_init_append(message, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &param)) {
        fprintf(stderr, "Error appending argument\n");
        return EXIT_FAILURE;
    }

    // Gửi yêu cầu và đợi phản hồi từ module
    ret = dbus_connection_send_with_reply(connection, message, &pending, -1);

    if (ret == DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER || ret == DBUS_REQUEST_NAME_REPLY_ALREADY_OWNER) {
        dbus_pending_call_block(pending);
    }
    else {
        fprintf(stderr, "Error sending D-Bus message\n");
        return EXIT_FAILURE;
    }

    // Lấy kết quả trả về từ module
    dbus_message_unref(message);
    dbus_pending_call_unref(pending);
    message = dbus_pending_call_steal_reply(pending);

    if (message == NULL) {
        fprintf(stderr, "Error getting D-Bus reply\n");
        return EXIT_FAILURE;
    }

    // Xử lý kết quả từ module
    dbus_message_iter_init(message, &args);

    char* response;
    dbus_message_iter_get_basic(&args, &response);
    printf("Response: %s\n", response);

    // Giải phóng bộ nhớ và đóng kết nối
    dbus_message_unref(message);
    dbus_connection_unref(connection);

    return EXIT_SUCCESS;
}

