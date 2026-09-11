#include <stddef.h>
#include <string.h>
// #include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

void rev(char *new, const char *text) {
    int len = (int)strlen(text);
    for (int i = 0; i < len; i++) {
        new[len - 1 - i] = text[i];
    }
    new[len] = '\0';
}

char* convbin(int n) {
    uint32_t x = (uint32_t)n;
    
    // Special case for 0
    if (x == 0) {
        char *s = (char*)malloc(2);
        if (!s) return NULL;
        s[0] = '0';
        s[1] = '\0';
        return s;
    }
    
    // Find the number of significant bits
    int bits = 0;
    uint32_t temp = x;
    while (temp > 0) {
        bits++;
        temp >>= 1;
    }

    char *s = (char*)malloc(bits + 1);
    if (!s) return NULL;

    for (int i = bits - 1, j = 0; i >= 0; i--, j++) {
        s[j] = ((x >> i) & 1u) ? '1' : '0';
    }
    s[bits] = '\0';
    return s;
}

void on_reverse_clicked(GtkButton *button, gpointer user_data) {
    GtkEntry *entry = GTK_ENTRY(user_data);
    GtkLabel *output = GTK_LABEL(g_object_get_data(G_OBJECT(button), "output-label"));
    const char *text = gtk_editable_get_text(GTK_EDITABLE(entry));
    if (!text) text = "";

    if (!atoi(text)) {
        gtk_label_set_text(output, "error: not number");
        return;
    }

    char new[512];
    rev(new, text);
    int is_decimal_pal = (strcmp(new, text) == 0);

    int n = atoi(text);
    char *bin = convbin(n);
    if (!bin) {
        gtk_label_set_text(output, "error: memory");
        return;
    }
    char newbin[512];
    rev(newbin, bin);
    int is_binary_pal = (strcmp(newbin, bin) == 0);
    free(bin);

    // Check both and output accordingly
    if (is_decimal_pal && is_binary_pal) {
        gtk_label_set_text(output, "palindrome in BOTH decimal and binary");
    } else if (is_binary_pal) {
        gtk_label_set_text(output, "palindrome in binary only");
    } else if (is_decimal_pal) {
        gtk_label_set_text(output, "palindrome in decimal only");
    } else {
        gtk_label_set_text(output, "not a palindrome");
    }
}

static void activate(GtkApplication *app, gpointer user_data) {
    (void)user_data;

    GtkWidget *window = gtk_application_window_new(app);
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_window_set_child(GTK_WINDOW(window), box);

    GtkWidget *entry = gtk_entry_new();
    GtkWidget *button = gtk_button_new_with_label("Check");
    GtkWidget *out_label = gtk_label_new("");

    gtk_box_append(GTK_BOX(box), entry);
    gtk_box_append(GTK_BOX(box), button);
    gtk_box_append(GTK_BOX(box), out_label);

    g_object_set_data(G_OBJECT(button), "output-label", out_label);
    g_signal_connect(button, "clicked", G_CALLBACK(on_reverse_clicked), entry);
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("org.udiskie2", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
