#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

int bitf(int x) {
  x |= x >> 1;
  x |= x >> 2;
  x |= x >> 4;
  x |= x >> 8;
  x |= x >> 16;
  return x - (x >> 1);
}

int is_pe(char **argv) {
  uint8_t header[4];
  size_t bytes_read;

  FILE *f_str = fopen(argv[2], "rb");

  if (f_str == nullptr) {
    cout << "Not PE\n";
    return 1;
  }
  fseek(f_str, 0x3C, SEEK_SET);
  bytes_read = fread(header, 1, 4, f_str);
  if (bytes_read != 4) {
    cout << "Not PE\n";
    return 1;
  }

  long pos = 0;
  for (int i = 0; i < 4; i++) {
    pos += header[i] * pow(16, 2 * i);
  }

  fseek(f_str, pos, SEEK_SET);

  uint8_t signature[4];
  bytes_read = fread(signature, 1, 4, f_str);
  if (bytes_read != 4) {
    cout << "Not PE\n";
    return 1;
  }
  fclose(f_str);

  if (signature[0] == 0x50 && signature[1] == 0x45 && signature[2] == 0x00 &&
      signature[3] == 0x00) {
    cout << "PE\n";
  } else {
    cout << "Not PE\n";
    return 1;
  }
  return 0;
}

int import_functions(char **argv) {
  uint8_t header[4];

  FILE *f_str = fopen(argv[2], "rb");

  fseek(f_str, 0x3C, SEEK_SET);
  fread(header, 1, 4, f_str);

  long pos = 0;
  for (int i = 0; i < 4; i++) {
    pos += header[i] * pow(16, 2 * i);
  }

  fseek(f_str, pos + 24 + 0x78, SEEK_SET);
  uint8_t import_table_rva[4];
  fread(import_table_rva, 1, 4, f_str);

  long res;
  long count = 0;
  long sr;
  long sraw;
  while (true) {
    fseek(f_str, count * 40 + pos + 24 + 240 + 0x8, SEEK_SET);
    uint8_t section_virtual_size[4];
    fread(section_virtual_size, 1, 4, f_str);

    fseek(f_str, count * 40 + pos + 24 + 240 + 0xC, SEEK_SET);
    uint8_t section_rva[4];
    fread(section_rva, 1, 4, f_str);

    fseek(f_str, count * 40 + pos + 24 + 240 + 0x14, SEEK_SET);
    uint8_t section_raw[4];
    fread(section_raw, 1, 4, f_str);

    long itr = 0;
    sr = 0;
    sraw = 0;
    long svs = 0;
    for (int i = 0; i < 4; i++) {
      itr += import_table_rva[i] * pow(16, 2 * i);
      sr += section_rva[i] * pow(16, 2 * i);
      svs += section_virtual_size[i] * pow(16, 2 * i);
      sraw += section_raw[i] * pow(16, 2 * i);
    }

    if (itr >= sr && itr <= (sr + svs)) {
      res = sraw + itr - sr;
      break;
    }
    count++;
  }

  long result;
  long result_func;
  long c = 0;
  while (true) {
    fseek(f_str, res + 0xC + c * 20, SEEK_SET);
    uint8_t data[4];
    fread(data, 1, 4, f_str);

    fseek(f_str, res + c * 20, SEEK_SET);
    uint8_t data_func[4];
    fread(data_func, 1, 4, f_str);

    bool flag = false;
    for (char i : data) {
      if (i != 0x00) {
        flag = true;
        break;
      }
    }
    if (!flag) {
      break;
    }

    long addr = 0;
    long addr_func = 0;
    for (int i = 0; i < 4; i++) {
      addr += data[i] * pow(16, 2 * i);
      addr_func += data_func[i] * pow(16, 2 * i);
    }

    result = sraw + addr - sr;
    result_func = sraw + addr_func - sr;
    fseek(f_str, result, SEEK_SET);

    char lib[1];
    while (true) {
      fread(lib, 1, 1, f_str);
      if (lib[0] == 0) {
        break;
      }
      cout << lib[0];
    }
    cout << "\n";

    int counter = 0;
    while (true) {
      fseek(f_str, result_func + counter * 8, SEEK_SET);
      uint8_t list[8];
      fread(list, 1, 8, f_str);

      bool fl = false;
      for (unsigned char i : list) {
        if (i != 0) {
          fl = true;
          break;
        }
      }
      if (!fl) {
        break;
      }

      long list_int = 0;
      for (int i = 0; i < 4; i++) {
        list_int += list[i] * pow(16, 2 * i);
      }

      if (bitf(list_int) == 1) {
        counter++;
        continue;
      }

      long res_list = list_int + sraw - sr + 2;
      fseek(f_str, res_list, SEEK_SET);

      char lib2[1];
      cout << "    ";
      while (true) {
        fread(lib2, 1, 1, f_str);
        if (lib2[0] == 0) {
          break;
        }
        cout << lib2[0];
      }
      cout << "\n";
      counter++;
    }
    ++c;
  }
  fclose(f_str);
  return 0;
}

int main(int argc, char **argv) {
  if (argc != 3) {
    return 1;
  }
  int res = 0;

  string q = argv[1];
  if (q == "is-pe") {
    res = is_pe(argv);
  } else if (q == "import-functions") {
    res = import_functions(argv);
  }

  return res ? 1 : 0;
}
