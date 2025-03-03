//
//  main.c
//  aeds2-tp2
//
//  Created by  Bruno Marra (3029), Gustavo Viegas (3026) e Heitor Passeado (3055) on 10/04/18.
//  Copyright © 2018 UFV Florestal. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <string.h>
#include "file.h"

#define MAX_FILE_NAME 100
#define BUFFER_SIZE 400

GtkLabel        *outputLabel;
GtkBuilder      *builder;
GtkWidget       *window, *fileChooserWidget, *fileLoadButtonWidget;
GtkFileChooser  *fileChooser;
GtkEntry        *valueInput;
GtkTextView     *outputLog;

TipoApontador Arvore;

/** 
 * @note Função principal do sistema  
 * @param  argc: argumentos passados na compilação
 * @param  *argv[]: array de novos argumentos
 * @retval 0
 */
int main(int argc, char *argv[]) {
    setvbuf(stdout, NULL, _IONBF, 0);
    Inicializa(&Arvore);
    gtk_init(&argc, &argv);
    
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "./projeto.glade", NULL);
    
    window = GTK_WIDGET(gtk_builder_get_object(builder, "mainWindow"));
    fileChooserWidget = GTK_WIDGET(gtk_builder_get_object(builder, "fileChooser"));
    fileLoadButtonWidget = GTK_WIDGET(gtk_builder_get_object(builder, "loadTreeButton"));
    
    outputLabel = GTK_LABEL(gtk_builder_get_object(builder, "outputLabel"));
    valueInput = GTK_ENTRY(gtk_builder_get_object(builder, "valueInput"));
    fileChooser = GTK_FILE_CHOOSER(gtk_builder_get_object(builder, "fileChooser"));
    outputLog = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "output"));
    
    gtk_builder_connect_signals(builder, 0);
    g_object_unref(builder);
    gtk_widget_show_all(window);
    gtk_main();
    
    fflush(stdout);
    return 0;
}

/** 
 * @note Evento chamado ao fechar a janela   
 * @retval None
 */
void on_mainWindow_destroy_event() {
    gtk_main_quit();
}

/** 
 * @note Adiciona um texto no log de eventos do sistema   
 * @param  text: array de caracteres que será inserido
 * @retval None
 */
void appendToLog(char* text) {
    GtkTextIter iter;
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(outputLog);
    GtkTextMark *mark = gtk_text_buffer_get_insert(buffer);
    gtk_text_buffer_get_iter_at_mark (buffer, &iter, mark);

    gtk_text_buffer_insert (buffer, &iter, "\n", 1);
    gtk_text_buffer_insert (buffer, &iter, text, -1);
}

/** 
 * @note Evento de clique no botão de carregar a árvore
 * @retval None
 */
void on_loadTreeButton_clicked () {
    char disableMessage[100];
    char message[BUFFER_SIZE];
    char *filePath = gtk_file_chooser_get_filename(fileChooser);
    
    strcpy(disableMessage, "So se pode carregar o arquivo uma vez. Rode o programa novamente se quiser carregar outro arquivo.");
    
    sprintf(message, "Carregando arvore...");
    appendToLog(message);
    
    if (CarregaDados(filePath, &Arvore, message)) {
        gtk_widget_set_sensitive(fileChooserWidget, 0);
        gtk_widget_set_sensitive(fileLoadButtonWidget, 0);
        gtk_widget_set_tooltip_text(fileChooserWidget, disableMessage);
        gtk_widget_set_tooltip_text(fileLoadButtonWidget, disableMessage);
    }

    appendToLog(message);
}

/** 
 * @note Pega o valor do campo de input   
 * @retval retorna o inteiro do valor digitado
 */
int getInputValue() {
    const gchar *text;
    text = gtk_entry_get_text(valueInput);
    return atoi(text);
}

/** 
 * @note Evento de clique no botão de busca   
 * @retval None
 */
void on_searchButton_clicked() {
    char message[BUFFER_SIZE];
    int comparisons = 0;
    int value = getInputValue();
    TipoRegistro target;
    target.Chave = value;
    
    Pesquisa(&target, Arvore, &comparisons, message);
    
    appendToLog(message);
}

/** 
 * @note Evento de clique no botão de inserção  
 * @retval None
 */
void on_insertButton_clicked() {
    char message[BUFFER_SIZE];
    int comparisons = 0;
    int value = getInputValue();
    
    TipoRegistro target;
    target.Chave = value;
    
    Insere(target, &Arvore, &comparisons, message);
    
    appendToLog(message);
}

/** 
 * @note Evento de clique no botão de remoção   
 * @retval None
 */
void on_removeButton_clicked() {
    char message[BUFFER_SIZE];
    int comparisons = 0;
    int value = getInputValue();
    
    Retira(value, &Arvore, &comparisons, message);
    
    appendToLog(message);
}

/** 
 * @note Evento de clique no botão de limpar o input   
 * @retval None
 */
void on_clearButton_clicked() {
    gtk_entry_set_text(valueInput, "");
}

/** 
 * @note Evento de clique no botão de exibir altura   
 * @retval None
 */
void on_heightButton_clicked() {
    char message[BUFFER_SIZE];
    int comparisons = 0;
    
    Altura(&Arvore, &comparisons, message);
    
    appendToLog(message);
}
