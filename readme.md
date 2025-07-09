# GAME GALAGA

Mô tả nội dung cần báo cáo của môn đồ án
> Sinh viên nên Fork từ dự án này

## GIỚI THIỆU

__Đề bài__: Mô phỏng trò chơi Galaga

__Sản phẩm:__
1. Điều khiển hướng di chuyển của phi thuyền bằng joystick
- Người chơi có thể điều khiển phi thuyền theo ý muốn bằng cần joystick (analog).
- Dữ liệu được đọc qua ADC ở chế độ polling, sau đó chuyển đổi thành vị trí trái, phải thời gian thực.
2. Âm thanh hiệu ứng
- Phát âm thanh mỗi khi người chơi bắn trúng quái(một tiếp pip) hoặc bắn chết quái(dựa trên số lượng máu của quái - hai tiếng pip).
3. Hiển thị giao diện bằng TouchGFX
- Giao diện sinh động, có hoạt ảnh khi đạn bay, quái bay
- Cập nhật liên tục trong tick() với tốc độ ổn định, tạo trải nghiệm chơi game mượt mà.
4. Logic trò chơi hoàn chỉnh
- Cập nhật điểm số và phát hiện gameover

- Ảnh chụp minh họa:\


## TÁC GIẢ

- Tên nhóm:VLH
- Thành viên trong nhóm
  |STT|Họ tên|MSSV|Công việc|
  |--:|--|--|--|
  |1|Trịnh Phúc Hữu|20215596|Thiết kế logic trò chơi|
  |2|Vũ Thị Mai Linh|20215608|Thiết kế touchGFX, viết báo cáo|
  |3|Hoàng Quốc Việt|20210936|Thiết kế lắp mạch, queue & task và còi|


## MÔI TRƯỜNG HOẠT ĐỘNG

## Kit sử dụng

`STM32F429 Discovery Kit`

---

## Module, linh kiện sử dụng

- **Joystick 2 trục**: điều khiển hướng di chuyển của phi thuyền 
- **Màn hình TFT LCD**: hiển thị giao diện trò chơi và hiệu ứng  
- **Buzzer (còi)**: phát âm thanh khi bắn
- **Nguồn USB 5V**: cấp nguồn cho kit hoạt động  

---

## SƠ ĐỒ SCHEMATIC

### Ghép nối STM32 với joystick

| STM32F429        | Joystick        |
|------------------|-----------------|
| VCC              | 3V              |
| GND              | GND             |
| PC3 (ADC1 IN13)   | Y               |
| PA5 (Digital input) | SW              |

---

### Ghép nối STM32 với còi Buzzer

| STM32F429 | Còi Buzzer |
|-----------|------------|
| PA7 (Output)     | (+)  |
| GND       | GND        |


### TÍCH HỢP HỆ THỐNG

## Phần cứng

### 1. Kit điều khiển chính

**STM32F429 Discovery Kit**  
Đóng vai trò xử lý toàn bộ logic trò chơi, hiển thị và điều khiển các thiết bị ngoại vi.

### 2. Joystick 2 trục

Được sử dụng để nhập điều khiển từ người chơi, điều khiển phi thuyền sang trái, phải.

### 3. Màn hình TFT LCD

Hiển thị đồ họa trò chơi, bao gồm trạng thái đạn, quái và giao diện Game Over.

### 4. Buzzer (còi)

Phát ra âm thanh khi bắn.

### 5. Nguồn USB 5V

Cấp nguồn cho toàn bộ hệ thống.

---

## Phần mềm

### 1. TouchGFX (cho STM32F429)

- Thiết kế giao diện người dùng (UI) với các màn hình:
  - Screen2: giao diện chơi game.
  - Screen3: giao diện Game Over.
- Xử lý hiệu ứng chuyển cảnh, cập nhật điểm số.
- Điều khiển logic trò chơi thông qua hàm `tick()` được gọi theo thời gian thực.

### 2. Firmware C/C++ (dựa trên STM32 HAL)

- Điều khiển phần cứng: LCD, buzzer.
- Đọc giá trị joystick qua ADC.
- Xác định hướng bắn từ giá trị joystick.
- Xử lý logic game như random đạn, kiểm tra va chạm, kết thúc game.
- Giao tiếp với TouchGFX thông qua các file như:
  - `ScreenView2.cpp`
  - `Model.cpp`

### 3. STM32CubeIDE

- Viết và quản lý code C/C++ sử dụng HAL.
- Tích hợp với mã được sinh ra từ TouchGFX.
- Hỗ trợ build project, debug và nạp firmware vào chip STM32F429.

---

## Joystick và ADC (phương pháp polling)

- Sử dụng 2 kênh ADC để đọc giá trị trục Y từ joystick.
- Phương pháp polling: giá trị được đọc trong hàm `handleTickEvent()` của TouchGFX.
- Dữ liệu ADC được xử lý để xác định:
  - Vị trí của phi thuyền.

---

## Timer và hệ thống tick của TouchGFX

- Cấu hình timer để tạo ngắt định kỳ.
- Mỗi tick sẽ thực hiện:
  - Đọc giá trị joystick.
  - Cập nhật vị trí đạn, phi thuyền và quái.
  - Kiểm tra va chạm.
  - Check máu và tính điểm.
  - Vẽ lại toàn bộ giao diện trò chơi.

### ĐẶC TẢ HÀM

### Hàm xử lý tick: `handleTickEvent()`

```cpp
/**
 * Hàm được gọi định kỳ mỗi khung hình (tick) của TouchGFX.
 * Chịu trách nhiệm cập nhật toàn bộ logic chính của màn chơi:
 *  - Đọc hướng bắn từ joystick.
 *  - Cập nhật vị trí đạn, vật thể.
 *  - Check máu và tính điểm.
 */
void Screen2View::handleTickEvent();
```

### Hàm kiểm tra va chạm: `checkCollision()`
```cpp
/**
 * Hàm kiểm tra va chạm giữa hai hình ảnh (sprites) trên màn hình.
 *
 * Hàm sử dụng giới hạn hình chữ nhật (bounding box) để xác định xem 
 * hai đối tượng `img1` và `img2` có giao nhau hay không.
 *
 * @param img1 Đối tượng hình ảnh thứ nhất (đạn).
 * @param img2 Đối tượng hình ảnh thứ hai (quái, phi thuyền).
 *
 * @return true nếu hai hình ảnh có phần diện tích chồng lên nhau (va chạm),
 *         false nếu không có va chạm.
 */
bool checkCollision (const touchgfx::Image& img1, const touchgfx::Image& img2);
```

  
### KẾT QUẢ
