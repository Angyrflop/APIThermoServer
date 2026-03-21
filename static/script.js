/*Copyright (c) goes to Jan Oliver Quant*/
const roomButtons = document.querySelectorAll(".room-btn");
const statusEl = document.getElementById("status");
const tempEl = document.getElementById("temperature");
const humidityEl = document.getElementById("humidity");
const lightEl = document.getElementById("light");
const lightCard = document.getElementById("light-card");

let autoUpdateInterval = null;
let currentRoom = null;

async function getReadings(room)
{
    setLoadingState(true);
    setStatus(`loading ${room}...`);

    try
    {
        const response = await fetch(`http://localhost:2048/getReading/${room}`);

        if (!response.ok)
        {
            throw new Error(`Something went wrong. ${response.status}`);
        }

        const data = await response.json();

        updateDisplay(data);
        setStatus(`Updated: ${new Date().toLocaleTimeString()}`);
    }
    catch   (error)
    {
        console.error("Fetch failed.", error);
        setStatus('Could not load data. Is the server running?');
    }
    finally
    {
        setLoadingState(false);
    }
}

function updateDisplay(data)
{
    tempEl.textContent = data.temperature?.toFixed(1) ?? "--";
    humidityEl.textContent = data.humidity?.toFixed(0) ?? "--";

    const isOn = data.light === true;
    lightEl.textContent = isOn ? "Ein" : "Aus";

    lightCard.classList.toggle("is-on", isOn);
    lightCard.classList.toggle("is-off", !isOn);
}

function setLoadingState(isLoading)
{
    document.querySelectorAll(".card").forEach(card => {
        card.classList.toggle("loading", isLoading);
    });
}

function setStatus(message)
{
    statusEl.textContent = message;
}

roomButtons.forEach(function(button)
{
    button.addEventListener("click", function()
    {
        const selectedRoom = button.dataset.room;
        
        if (currentRoom !== selectedRoom && autoUpdateInterval !== null)
        {
            clearInterval(autoUpdateInterval);
            autoUpdateInterval = null;
        }
        
        currentRoom = selectedRoom;
        roomButtons.forEach(btn => btn.classList.remove("active"));
        button.classList.add("active");
        
        
        getReadings(selectedRoom);
        
        
        if (autoUpdateInterval === null)
        {
            autoUpdateInterval = setInterval(() => {
                getReadings(currentRoom);
            }, 5000);
        }
    });
});