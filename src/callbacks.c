#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <gtk/gtk.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include "callbacks.h"
#include "interface.h"
#include "support.h"



void
on_quit1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
gtk_main_quit();
}



void
on_about1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  GtkWidget *dialog1;
  dialog1= create_dialog1 ();
  gtk_widget_show (dialog1);
}



gboolean
on_window1_destroy_event               (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
gtk_main_quit();
  return FALSE;
}


gboolean
on_window1_delete_event                (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
gtk_main_quit();
  return FALSE;
}

gboolean
on_dialog1_delete_event                (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
GtkWidget *dialog1;  
dialog1 = lookup_widget (GTK_WIDGET (widget), "dialog1");
gtk_widget_destroy(dialog1);  	
return FALSE;
}


gboolean
on_dialog1_destroy_event               (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
GtkWidget *dialog1;  
dialog1 = lookup_widget (GTK_WIDGET (widget), "dialog1");
gtk_widget_destroy(dialog1);  	

return FALSE;
}


void
on_okbutton1_released                  (GtkButton       *button,
                                        gpointer         user_data)
{
//dialog butonu basilmasi durumunda
GtkWidget *dialog1;  
dialog1 = lookup_widget (GTK_WIDGET (button), "dialog1");
gtk_widget_destroy(dialog1);  	
}


/*********************************************************/

void
on_button1_pressed                     (GtkButton       *button,
                                        gpointer         user_data)
{
   	
   gchar *deger[] = { "", "" };
   FILE *dosyatur;
   FILE *dosyaing;
   int konum, uzunluk, KacTaneBulundu; // KacTaneBulundu, arama özelliği için kullanılacak
   gchar ing[255];
   gchar tur[9192];
   gchar *EvDizini, *DosyaKonumu; // $HOME degiskeni ve buna bağımlı dosya konumu için
   gchar *aranan, *arananG; // arananG(osterge)'yi aranan içinde dolasmak için kullanacağız
   GtkWidget *textview1;
   GtkWidget *entry1;
   GtkTextBuffer *buffer;
   GtkTextIter iter;
   GdkFont *font;
   GdkColor *color;
   gboolean italic, bold, underline;
   gchar *utf8;
   GtkTextIter start, end;
   gint giris=0;

   EvDizini = getenv("HOME");
   if( (DosyaKonumu = (char *) malloc( (strlen(EvDizini) + strlen("/.gtksozluk2/") + strlen("ingtur-ing") + 1) * sizeof(char) )) == NULL ) {
	g_printerr(_("DosyaKonumu değişkeni için yeterli yer ayrılamadı"));
	exit(EXIT_FAILURE);
   }
   sprintf(DosyaKonumu, "%s/.gtksozluk2/ingtur-ing", EvDizini);
   textview1 = lookup_widget (GTK_WIDGET (button), "textview1");
   entry1 = lookup_widget (GTK_WIDGET (button), "entry1");
   aranan = gtk_entry_get_text (entry1);
   
   buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (textview1));
   gtk_text_buffer_get_bounds (buffer, &start, &end);
   gtk_text_buffer_delete (buffer, &start, &end);
   
   //dosya acma islemleri

   if( (dosyaing = fopen(DosyaKonumu, "r")) == NULL ) { // Ev dizininde gerekli dosya bulunamazsa
	  
   	if ((dosyaing = fopen ("/usr/local/share/gtksozluk2/ingtur-ing", "r")) == NULL) { // Genel dosya da açılamazsa
		gtk_text_buffer_get_end_iter (GTK_TEXT_BUFFER (buffer), &start);
		gtk_text_buffer_insert (buffer, &start, _("Gerekli dosyalardan ingtur-ing bulunamadı."), -1);
      		g_printerr (_("/usr/local/share/gtksozluk2/ingtur-ing dosyasi bulunamadi\n"));
		return;
	}	
		
   }
   
   sprintf(DosyaKonumu, "%s/.gtksozluk2/ingtur-tur", EvDizini); // Diğer dosya için konumu tanımlayalım
   
   if( (dosyatur = fopen(DosyaKonumu, "r")) == NULL ) { // Ev dizininde gerekli dosya bulunamazsa
	  
   	if ((dosyatur = fopen ("/usr/local/share/gtksozluk2/ingtur-tur", "r")) == NULL) { // Genel dosya da açılamazsa
		gtk_text_buffer_get_end_iter (GTK_TEXT_BUFFER (buffer), &start);
		gtk_text_buffer_insert (buffer, &start, _("Gerekli dosyalardan ingtur-tur bulunamadı."), -1);
      		g_printerr (_("/usr/local/share/gtksozluk2/ingtur-tur dosyasi bulunamadi\n"));
		return;
	}	
		
   }

   if( strchr(g_strstrip(aranan), '*') ) { // Arama özelliği
	   KacTaneBulundu = 0;

	   if(strlen(aranan) == 1 ) { // Küçük bir kontrol ;)
   		gtk_text_buffer_insert(buffer, &start, _("Sorunlu arama kriteri (Tek karakter)"), -1);
		return;
	   }

	    	  if( (aranan[strlen(aranan)-1] == '*') && (*aranan == '*' )) { // 1. durum
			   aranan[strlen(aranan)-1] = '\0'; // Sondaki yıldızı uçur
			   arananG = aranan+1; // Yıldız karakterini atla
   			   gtk_text_buffer_insert_with_tags_by_name(buffer, &start, arananG, -1, "bold", "italic", "blue_foreground", "underline",  NULL);
   			   gtk_text_buffer_insert(buffer, &start, "\n", -1);

		   	 	                             
	   		   while( !feof(dosyaing) )
        		   {
                 		fgets (ing, 255, dosyaing);
                 
                     	   	if ( strstr(g_strstrip(ing), g_strstrip(arananG)) != NULL )
            	         	{         	
					KacTaneBulundu++;
                 		
                	 		utf8=g_convert (arananG, -1, "UTF-8", "ISO8859-9", NULL, NULL, NULL);
                 			gtk_text_buffer_get_end_iter (GTK_TEXT_BUFFER (buffer), &start);
                 			gtk_text_buffer_insert (buffer, &start, ing, -1);
                 			gtk_text_buffer_insert (buffer, &start, "\n", -1);
                 
                       	  	}
                   	   }

		  } // 1. durum sonu

		  else if( aranan[strlen(aranan)-1] == '*' ) { // 2. durum
			   aranan[strlen(aranan)-1] = '\0'; // Sondaki yıldızı uçur
   			   gtk_text_buffer_insert_with_tags_by_name(buffer, &start, aranan, -1, "bold", "italic", "blue_foreground", "underline",  NULL);
   			   gtk_text_buffer_insert(buffer, &start, "\n", -1);
               	   
	   		   while( !feof(dosyaing) )
        		   {
                 		fgets (ing, 255, dosyaing);
                 
                     	   	if ( !strncmp(g_strstrip(ing), g_strstrip(aranan), strlen(aranan)) )
            	         	{         	
					KacTaneBulundu++;
                 		
                	 		utf8=g_convert (aranan, -1, "UTF-8", "ISO8859-9", NULL, NULL, NULL);
                 			gtk_text_buffer_get_end_iter (GTK_TEXT_BUFFER (buffer), &start);
                 			gtk_text_buffer_insert (buffer, &start, ing, -1);
                 			gtk_text_buffer_insert (buffer, &start, "\n", -1);
                 
                       	  	}
			   }
		   
	   	   } // 2. durum sonu

		   else if( aranan[0] == '*' ) { // 3. durum
			   arananG = aranan+1; // Yıldız karakterini atla
   			   gtk_text_buffer_insert_with_tags_by_name(buffer, &start, arananG, -1, "bold", "italic", "blue_foreground", "underline",  NULL);
   			   gtk_text_buffer_insert(buffer, &start, "\n", -1);
			   
	   		   while( !feof(dosyaing) )
        		   {
                 		fgets (ing, 255, dosyaing);
                 
			   	if ( !SondanKarsilastir(g_strstrip(ing), g_strstrip(arananG) )) {
					KacTaneBulundu++;
                 		
                	 		utf8=g_convert (aranan, -1, "UTF-8", "ISO8859-9", NULL, NULL, NULL);
                 			gtk_text_buffer_get_end_iter (GTK_TEXT_BUFFER (buffer), &start);
                 			gtk_text_buffer_insert (buffer, &start, ing, -1);
                 			gtk_text_buffer_insert (buffer, &start, "\n", -1);
                 
                       	  	}
			   }
		   } // 3. durum sonu

    if( KacTaneBulundu == 0 ) // Sonuc yoksa
	    gtk_text_buffer_insert (buffer, &start, _("Aradığınız kritere uygun bir kelime bulunamadı."), -1);
   	    // NTPL'ye selamlar olsun, yukarıdaki cümle için
    
    fclose(dosyaing);
    return;			   	   
    } // Arama özelliği son
   		
     while( !feof(dosyaing) )
     {
       fgets (ing, 255, dosyaing);
       fgets (tur, 9192, dosyatur); 
   
       	    if ( strcmp ( g_strstrip(ing),  g_strstrip(aranan) )==0 )
       		{         	
   		
   		utf8=g_convert (tur, -1, "UTF-8", "ISO8859-9", NULL, NULL, NULL);
   		gtk_text_buffer_get_end_iter (GTK_TEXT_BUFFER (buffer), &start);
   		//aranan kelime yatik bold ve renkli olarak yazilacak
   		//gtk_text_buffer_insert_with_tags_by_name (buffer, &start, "bold", -1, "bold", NULL);
   		gtk_text_buffer_insert_with_tags_by_name (buffer, &start, ing, -1, "bold", "italic", "blue_foreground", "underline",  NULL);
   		gtk_text_buffer_insert (buffer, &start, " :\n", -1);
   
   		//turkce aciklamasi
   		gtk_text_buffer_insert (GTK_TEXT_BUFFER (buffer), &start, utf8, -1);  
   		
   		//gtk_text_buffer_set_text (buffer,  utf8, -1);
   		//gtk_text_buffer_set_text (buffer,  "deneme", -1);
   		giris=1;	
      		}
   	}
   
   if (giris==0) {  gtk_text_buffer_get_end_iter (GTK_TEXT_BUFFER (buffer), &start);		
   		gtk_text_buffer_insert (buffer, &start, " Aranan kelime bulunamadi.", -1);
   	     }
   
   fclose (dosyatur);
   fclose (dosyaing);
   
   return;

}

void
on_button1_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
on_button1_pressed    (button,    user_data);
 return;
}

void
on_window1_show                        (GtkWidget       *widget,
                                        gpointer         user_data)
{
GtkWidget *textview1;
GtkTextBuffer *buffer;
GtkTextIter iter;
GtkTextIter start, end;

textview1 = lookup_widget (GTK_WIDGET (widget), "textview1");
buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (textview1));
gtk_text_buffer_get_bounds (buffer, &start, &end);
gtk_text_buffer_delete (buffer, &start, &end);

//kullanilan taglar
gtk_text_buffer_create_tag (buffer, "italic", "style", PANGO_STYLE_ITALIC, NULL);
gtk_text_buffer_create_tag (buffer, "bold", "weight", PANGO_WEIGHT_BOLD, NULL);  
gtk_text_buffer_create_tag (buffer, "big", "size", 20 * PANGO_SCALE, NULL);
gtk_text_buffer_create_tag (buffer, "xx-small", "scale", PANGO_SCALE_XX_SMALL, NULL);
gtk_text_buffer_create_tag (buffer, "x-large", "scale", PANGO_SCALE_X_LARGE, NULL);
gtk_text_buffer_create_tag (buffer, "monospace", "family", "monospace", NULL);
gtk_text_buffer_create_tag (buffer, "blue_foreground",  "foreground", "blue", NULL);  
gtk_text_buffer_create_tag (buffer, "underline", "underline", PANGO_UNDERLINE_SINGLE, NULL);

return;
}

int
SondanKarsilastir(char *ilk, char *ikinci)
{
	int ilkUz, ikinciUz; // tek tek karakter kontrolü için kullanılacaklar

	ilkUz = strlen(ilk) - 1 ;
	ikinciUz = strlen(ikinci) - 1;

	while( (ikinciUz >= 0 ) ) {
		if( ilk[ilkUz] != ikinci[ikinciUz] )
			return ilk[ilkUz] - ikinci[ikinciUz];
		ilkUz--;
		ikinciUz--;
	}

	return 0;
}	
