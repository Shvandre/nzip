from sys import argv
import subprocess

# CRC64 ISO polynomial
POLY64REV = 0xC96C5795D7870F42
INITIALCRC = 0xFFFFFFFFFFFFFFFF


def get_crc32_from_cksum(file_path):
    try:
        # Вызов утилиты cksum и получение вывода
        result = subprocess.run(['cksum', file_path], capture_output=True, text=True, check=True)
        # Разделение вывода на части и извлечение первого числа
        crc32 = result.stdout.split()[0]
        return int(crc32)
    except subprocess.CalledProcessError as e:
        print(f"Error calling cksum: {e}")
        return None
    except ValueError as e:
        print(f"Error parsing cksum output: {e}")
        return None


def find_and_replace_pattern(file_path, pattern: bytes):
    # Чтение бинарного файла
    with open(file_path, 'rb') as f:
        file_data = f.read()

    # Вычисление CRC32 хэш-суммы файла
    file_hash = get_crc32_from_cksum(file_path)
    file_hash_bytes = file_hash.to_bytes(4, 'little')

    # Поиск паттерна в файле

    start_index = file_data.find(pattern)
    if start_index == -1:
        print("Pattern not found in the file.")
        return

    # Заменяем паттерн на хэш-сумму
    end_index = start_index + len(pattern)
    new_file_data = file_data[:start_index] + file_hash_bytes + file_data[end_index:]

    # Запись измененного файла
    with open(file_path, 'wb') as f:
        f.write(new_file_data)
    print("Pattern replaced successfully.", "File hash:", file_hash)


# Пример использования

pattern = 0xAABBCCDD.to_bytes(4, 'big')
file_path = argv[1]

find_and_replace_pattern(file_path, pattern)
