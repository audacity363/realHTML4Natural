// vim: tabstop=4

/***************************************************************************/
/**
 * @file hexdump.h
 * @brief Header-File HEX-Dump-Modul zur Erzeugung eines Hex-Dumps mit den Hex- und
 * ASCII-Daten eines Streams.
 *
 * @author Dieter Engemann hans-dieter.engemann@hydro.com
 *
 * Projekt      : Library fuer Hex-Dump\n
 * Company      : Hydro Aluminum Rolled Products GmbH\n
 *
 *
 * @date 14.12.1993 erste Implementierung
 *
 ***************************************************************************/
void logHexDump(void *data, int size, FILE *fp);
