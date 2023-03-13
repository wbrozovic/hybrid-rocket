function areYouSure(event) {
    if (confirm("Are you sure?")) {
        finalConfirmation();
    } else {
    }
}

function finalConfirmation() {
    var countdownModal = document.getElementById("countdown-modal");
    countdownModal.style.visibility = "visible";

    var modalOverlay = document.getElementById("modal-overlay");
    modalOverlay.style.visibility = "visible";
}

function cancelIgnition() {
    var countdownModal = document.getElementById("countdown-modal");
    countdownModal.style.visibility = "hidden";
}

function beginCountDown() {
    var countdownModal = document.getElementById("countdown-modal");
    var modalOverlay = document.getElementById("modal-overlay");
    countdownModal.innerHTML = `
        <div id='countdown' class='confirm-modal__content h1'>
            10
        </div>
        <button class='secondary-btn' style='margin-top: 1rem;' onclick='cancelIgnition()'>
            CANCEL
        </button>
        `;
    var countdown = document.getElementById("countdown");
    var count = 10;
    var counter = setInterval(timer, 1000);
    function timer() {
        count = count - 1;
        if (count <= 0) {
            clearInterval(counter);
            countdown.style.visibility = "hidden";
            modalOverlay.style.visibility = "hidden";
            resetConfimationModal();
            return;
        }
        countdown.innerHTML = count;
    }
}

function resetConfimationModal() {
    var countdownModal = document.getElementById("countdown-modal");
    countdownModal.innerHTML = `
    <div id="countdown-modal__content" class="confirm-modal__content">
    <h1 id="countdown-modal__header" class="h1">Are you sure?</h1>
    <p id="countdown-modal__text" class="p">
      This will start the 10 second countdown and ignite the engine.
    </p>
    <div id="countdown-modal__buttons" class="modal-btn-container">
      <button
        id="countdown-modal__cancel"
        class="cancel-btn secondary-btn"
        onclick="cancelIgnition()"
      >
        Cancel
      </button>
      <button
        id="countdown-modal__confirm"
        class="confirm-btn danger-btn"
        onclick="beginCountDown()"
      >
        Confirm
      </button>
    </div>
        `;
    countdownModal.style.visibility = "hidden";
}

function setNumberSpinners() {
    var numberSpinners = document.querySelectorAll(".spinner-holder");

    numberSpinners.forEach((spinner) => {
        let id = spinner.attributes['spinner-id'].value;

        customSpinnerHtml = `
        <div style="position: relative; float: right; position: relative; height: 100%;">
          <button input-id=${id} class="number-spinner__btn-up number-spinner-btn" onclick="increment(event)">+</button>
          <button input-id=${id} class="number-spinner__btn-down number-spinner-btn" onclick="decrement(event)">-</button>
        </div>
        `;

        spinner.insertAdjacentHTML("beforeend", customSpinnerHtml);
    });
}

setNumberSpinners();

function increment(event) {
    var inputId = event.currentTarget.attributes['input-id'].value;
    var input = document.getElementById(inputId);
    var value = parseInt(Number(input.value.replace('%', '')), 10);
    value = isNaN(value) ? 0 : value;
    if (value >= 100) return
    value++;
    input.value = value + '%';

    updateGuage(value);
}

function decrement(event) {
    var inputId = event.currentTarget.attributes['input-id'].value;
    var input = document.getElementById(inputId);
    var value = parseInt(Number(input.value.replace('%', '')), 10);
    value = isNaN(value) ? 0 : value;
    value < 1 ? value = 1 : '';
    if (value <= 0) return
    value--;
    input.value = value + '%';

    updateGuage(value);
}

function appendPercent(event) {
    var input = event.currentTarget;
    var value = parseInt(Number(input.value.replace('%', '')), 10);
    value = isNaN(value) ? 0 : value;
    input.value = value + '%';

    updateGuage(value);
}

function updateGuage(value) {
    var gauge = document.getElementById("gauge-color-overlay");
    gauge.style.width = value + '%';

    if (value >= 100) {
      gauge.style.borderTopRightRadius = '4px';
      gauge.style.borderBottomRightRadius = '4px';
    } else {
      gauge.style.borderTopRightRadius = '0';
      gauge.style.borderBottomRightRadius = '0';
    }
}